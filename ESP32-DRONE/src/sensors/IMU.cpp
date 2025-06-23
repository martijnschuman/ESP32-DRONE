// sensors/IMU.cpp
#include "IMU.h"

// MPU‑6050 register definitions
#define MPU_ADDR       0x68
#define REG_PWR_MGMT1  0x6B
#define REG_CONFIG     0x1A
#define REG_GYRO_CFG   0x1B
#define REG_ACCEL_CFG  0x1C
#define REG_ACCEL_DATA 0x3B

const float GYRO_SCALE = 131.0f;
const float ACC_SCALE  = 16384.0f; 

static float accelBias[3] = {0, 0, 0}; // X, Y, Z accelerometer biases in g
static float gyroBias[3]  = {0, 0, 0}; // X, Y, Z gyroscope biases in dps

float roll  = 0.0f;
float pitch = 0.0f;
float yaw   = 0.0f;
float currentYawRate = 0.0f; // °/s  (updated each IMU cycle)

static Madgwick filter; // -> MadgwickAHRS.h -> MadgwickAHRS.cpp -> betaDef was set to 0.7f
static unsigned long lastYawMicros = 0;

// Helper to write a single byte to a register on the MPU‑6050
static inline void writeReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission();
}

// Helper to read a burst of bytes from the MPU‑6050
static inline void readBurst(uint8_t startReg, uint8_t *buf, uint8_t len) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(startReg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, len, (uint8_t)true);
    for (uint8_t i = 0; i < len; ++i) buf[i] = Wire.read();
}

// Helper to read a big-endian 16-bit from a byte buffer
static inline int16_t read16(const uint8_t *buf, uint8_t idx) {
    return (int16_t)((buf[idx] << 8) | buf[idx + 1]);
}

void setupIMU() {
    Wire.begin();
    delay(100);

    // Wake up & set gyro/accel ranges
    writeReg(REG_PWR_MGMT1, 0x00);          // wake
    writeReg(REG_GYRO_CFG,   0x00);         // ±250 °/s
    writeReg(REG_ACCEL_CFG,  0x00);         // ±2 g
    writeReg(REG_CONFIG,     0x03);         // DLPF @ 44/42Hz

    filter.begin(100.0f);                  // 100 Hz update

    Serial.println("MPU-6050 initialised (100 Hz)");
}

// Take offsets for roll/pitch & determine gyro Z bias while completely still
void calibrateIMUOffsets() {
    Serial.println(F("Calibrating IMU (keep drone level and still) ..."));
    delay(1000);

    constexpr int numSamples = 1000;
    float accSum[3] = {0.0f, 0.0f, 0.0f};
    float gyroSum[3] = {0.0f, 0.0f, 0.0f};

    for (int i = 0; i < numSamples; ++i) {
        uint8_t raw[14];
        readBurst(REG_ACCEL_DATA, raw, 14);

        accSum[0] += static_cast<float>(read16(raw, 0));
        accSum[1] += static_cast<float>(read16(raw, 2));
        accSum[2] += static_cast<float>(read16(raw, 4));
        gyroSum[0] += static_cast<float>(read16(raw, 8));
        gyroSum[1] += static_cast<float>(read16(raw, 10));
        gyroSum[2] += static_cast<float>(read16(raw, 12));

        delay(2);
    }

    for (int i = 0; i < 3; ++i) {
        accelBias[i] = (accSum[i] / numSamples) / ACC_SCALE;
        gyroBias[i]  = (gyroSum[i] / numSamples) / GYRO_SCALE;
    }

    // Z-accelerometer should be 1g when level. Adjust bias.
    accelBias[2] -= 1.0f;

    lastYawMicros = micros();
    yaw = 0.0f;

    Serial.println("IMU calibration complete.");
    Serial.printf(" Accel Biases (g): X: %.4f, Y: %.4f, Z: %.4f\n", accelBias[0], accelBias[1], accelBias[2]);
    Serial.printf(" Gyro Biases (dps): X: %.4f, Y: %.4f, Z: %.4f\n", gyroBias[0], gyroBias[1], gyroBias[2]);
}

void updateIMU() {
    // Read accel & gyro in one 14‑byte burst
    uint8_t raw[14];
    readBurst(REG_ACCEL_DATA, raw, 14);

    // Get raw values from the buffer
    int16_t ax = read16(raw, 0);
    int16_t ay = read16(raw, 2);
    int16_t az = read16(raw, 4);
    int16_t gx = read16(raw, 8);
    int16_t gy = read16(raw, 10);
    int16_t gz = read16(raw, 12);

    // Convert to physical units
    float ax_g = ax / ACC_SCALE - accelBias[0];
    float ay_g = ay / ACC_SCALE - accelBias[1];
    float az_g = az / ACC_SCALE - accelBias[2];
    float gx_dps = gx / GYRO_SCALE - gyroBias[0];
    float gy_dps = gy / GYRO_SCALE - gyroBias[1];
    float gz_dps = gz / GYRO_SCALE - gyroBias[2];

    // Update Madgwick filter with gyro and accelerometer data
    filter.updateIMU(gx_dps * DEG_TO_RAD,
                     gy_dps * DEG_TO_RAD,
                     gz_dps * DEG_TO_RAD,
                     ax_g, ay_g, az_g);

    // Raw orientation from filter ──────────────────────────
    roll  = filter.getRoll();
    pitch = filter.getPitch();

    // Manually integrate yaw
    unsigned long now = micros();
    if (lastYawMicros == 0) lastYawMicros = now;   // first call guard
    float dt = (now - lastYawMicros) * 1e-6f;      // seconds
    lastYawMicros = now;

    yaw += gz_dps * dt;
    if (yaw >  180.0f) yaw -= 360.0f;
    if (yaw < -180.0f) yaw += 360.0f;

    // ─── Debug (comment out in flight) ────────────────────────
    // Serial.printf("IMU: X %.2f  Y %.2f  Z %.2f\n", gx_dps, gy_dps, gz_dps);
    // Serial.printf("IMU: X %.2f  Y %.2f  Z %.2f\n", ax_g, ay_g, az_g);
    // Serial.printf("CM %lu R %.2f  P %.2f  Y %.2f\n", currentMillis, roll, pitch, yaw);
}
