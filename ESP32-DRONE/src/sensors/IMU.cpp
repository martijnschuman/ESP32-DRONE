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

static Madgwick filter; // MadgwickAHRC.cpp -> beta -> 0.2f
static unsigned long lastUpdateMicros = 0;

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

    writeReg(REG_CONFIG, 0x03);   // 44 Hz DLPF

    filter.begin(200.0f);         // 200 Hz

    lastUpdateMicros = micros();
}

void calibrateIMUOffsets() {
    const int N = 1000;
    float gxSum=0, gySum=0, gzSum=0;
    float rSum=0, pSum=0;

    delay(2000);

    for(int i=0;i<N;i++) {
        uint8_t raw[14];
        readBurst(REG_ACCEL_DATA, raw, 14);

        int16_t gx = (raw[8] << 8) | raw[9];
        int16_t gy = (raw[10] << 8) | raw[11];
        int16_t gz = (raw[12] << 8) | raw[13];

        gxSum += gx;
        gySum += gy;
        gzSum += gz;

        updateIMU();
        rSum += roll;
        pSum += pitch;

        delay(2);
    }

    gyroBias[0] = (gxSum/N)/GYRO_SCALE;
    gyroBias[1] = (gySum/N)/GYRO_SCALE;
    gyroBias[2] = (gzSum/N)/GYRO_SCALE;

    rollOffset  = rSum/N;
    pitchOffset = pSum/N;

    yaw = 0;
}

void updateIMU() {
    uint8_t raw[14];
    readBurst(REG_ACCEL_DATA, raw, 14);

    int16_t ax = (raw[0] << 8) | raw[1];
    int16_t ay = (raw[2] << 8) | raw[3];
    int16_t az = (raw[4] << 8) | raw[5];
    int16_t gx = (raw[8] << 8) | raw[9];
    int16_t gy = (raw[10] << 8) | raw[11];
    int16_t gz = (raw[12] << 8) | raw[13];

    float ax_g = ax / ACC_SCALE;
    float ay_g = ay / ACC_SCALE;
    float az_g = az / ACC_SCALE;

    rollRate  = (gx / GYRO_SCALE) - gyroBias[0];
    pitchRate = (gy / GYRO_SCALE) - gyroBias[1];
    yawRate   = (gz / GYRO_SCALE) - gyroBias[2];

    unsigned long now = micros();
    float dt = (now - lastUpdateMicros) * 1e-6f;
    lastUpdateMicros = now;

    yaw += yawRate * dt;
    if (yaw > 180) yaw -= 360;
    if (yaw < -180) yaw += 360;

    filter.updateIMU(
        rollRate * DEG_TO_RAD,
        pitchRate * DEG_TO_RAD,
        yawRate * DEG_TO_RAD,
        ax_g, ay_g, az_g
    );

    roll  = filter.getRoll()  - rollOffset;
    pitch = filter.getPitch() - pitchOffset;
}