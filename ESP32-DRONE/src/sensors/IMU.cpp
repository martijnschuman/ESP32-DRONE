#include "IMU.h"

#define MPU_ADDR       0x68
#define REG_PWR_MGMT1  0x6B
#define REG_CONFIG     0x1A
#define REG_GYRO_CFG   0x1B
#define REG_ACCEL_CFG  0x1C
#define REG_ACCEL_DATA 0x3B

const float GYRO_SCALE = 131.0f;      // ±250 dps
const float ACC_SCALE  = 16384.0f;    // ±2g

// Orientation outputs
float roll = 0.0f;
float pitch = 0.0f;
float yaw = 0.0f;

// Gyro rates (deg/sec)
float rollRate = 0.0f;
float pitchRate = 0.0f;
float yawRate = 0.0f;

static float gyroBias[3] = {0,0,0};
static float rollOffset = 0;
static float pitchOffset = 0;

static Madgwick filter; // MadgwickAHRC.cpp -> beta -> 0.6f

static inline void writeReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

static inline void readBurst(uint8_t startReg, uint8_t *buf, uint8_t len) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(startReg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, len, true);
    for (uint8_t i = 0; i < len; ++i)
        buf[i] = Wire.read();
}

void setupIMU() {
    Wire.begin();
    delay(100);

    writeReg(REG_PWR_MGMT1, 0x00);
    writeReg(REG_GYRO_CFG,  0x00);
    writeReg(REG_ACCEL_CFG, 0x00);

    writeReg(REG_CONFIG, 0x02);   // 93 Hz DLPF

    filter.begin(200.0f);         // 200 Hz
}

// --- Helper: parse a 14-byte MPU burst into int16 values ---
struct ImuRawSample {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
};

static inline ImuRawSample parseBurst14(const uint8_t raw[14]) {
    ImuRawSample s{};
    s.ax = (int16_t)((raw[0] << 8) | raw[1]);
    s.ay = (int16_t)((raw[2] << 8) | raw[3]);
    s.az = (int16_t)((raw[4] << 8) | raw[5]);
    s.gx = (int16_t)((raw[8] << 8) | raw[9]);
    s.gy = (int16_t)((raw[10] << 8) | raw[11]);
    s.gz = (int16_t)((raw[12] << 8) | raw[13]);

    return s;
}

// --- Core IMU update from a provided sample (no I2C read here) ---
static inline void updateIMUFromSample(const ImuRawSample& s, float dt = 0.005f) {
    // accel -> g
    const float ax_g = (float)s.ax / ACC_SCALE;
    const float ay_g = (float)s.ay / ACC_SCALE;
    const float az_g = (float)s.az / ACC_SCALE;

    // gyro -> deg/s with bias removed
    rollRate  = ((float)s.gx / GYRO_SCALE) - gyroBias[0];
    pitchRate = ((float)s.gy / GYRO_SCALE) - gyroBias[1];
    yawRate   = ((float)s.gz / GYRO_SCALE) - gyroBias[2];

    // yaw integrate (deg)
    yaw += yawRate * dt;
    if (yaw > 180.0f)  yaw -= 360.0f;
    if (yaw < -180.0f) yaw += 360.0f;

    // Madgwick expects rad/s for gyro inputs
    filter.updateIMU(
        rollRate  * DEG_TO_RAD,
        pitchRate * DEG_TO_RAD,
        yawRate   * DEG_TO_RAD,
        ax_g, ay_g, az_g);

    // fused angles + offsets
    roll  = filter.getRoll()  - rollOffset;
    pitch = filter.getPitch() - pitchOffset;
}

// --- Public IMU update: exactly one I2C burst read ---
void updateIMU(float dt) {
    uint8_t raw[14];
    readBurst(REG_ACCEL_DATA, raw, 14);
    const ImuRawSample s = parseBurst14(raw);
    updateIMUFromSample(s, dt);
}

// --- Calibration: exactly one I2C burst read per iteration ---
// Keeps rollRate/pitchRate/yawRate updated too (via updateIMUFromSample),
// while accumulating gyro bias + roll/pitch offsets.
void calibrateIMUOffsets() {
    const int N = 1000;

    float gxSum = 0.0f, gySum = 0.0f, gzSum = 0.0f;
    float rSum  = 0.0f, pSum  = 0.0f;

    // Ensure consistent cadence during calibration
    delay(2000);

    // Temporarily zero biases/offsets so updateIMUFromSample uses raw gyro scale
    // (gyroBias is what we're computing; rollOffset/pitchOffset computed after).
    const float oldGyroBias0 = gyroBias[0], oldGyroBias1 = gyroBias[1], oldGyroBias2 = gyroBias[2];
    const float oldRollOffset = rollOffset, oldPitchOffset = pitchOffset;

    gyroBias[0] = gyroBias[1] = gyroBias[2] = 0.0f;
    rollOffset = 0.0f;
    pitchOffset = 0.0f;

    for (int i = 0; i < N; i++) {
        uint8_t raw[14];
        readBurst(REG_ACCEL_DATA, raw, 14);
        const ImuRawSample s = parseBurst14(raw);

        // Accumulate raw gyro (LSB) for bias estimation
        gxSum += (float)s.gx;
        gySum += (float)s.gy;
        gzSum += (float)s.gz;

        // Update filter + rates from this SAME sample (no extra readBurst)
        updateIMUFromSample(s);

        // Accumulate fused angles (with offsets currently zero)
        rSum += roll;
        pSum += pitch;

        delay(5); // ~200 Hz
    }

    // Compute gyro bias in deg/s
    gyroBias[0] = (gxSum / (float)N) / GYRO_SCALE;
    gyroBias[1] = (gySum / (float)N) / GYRO_SCALE;
    gyroBias[2] = (gzSum / (float)N) / GYRO_SCALE;

    // Compute roll/pitch offsets (deg)
    rollOffset  = rSum / (float)N;
    pitchOffset = pSum / (float)N;

    filter.begin(200.0f);          // reset internal state/timebase
    yaw = 0.0f;
}