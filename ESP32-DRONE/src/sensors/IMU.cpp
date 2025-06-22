// IMU.cpp – MPU‑6050 only (no magnetometer)
//--------------------------------------------------------------------
//  • Roll & pitch  : Madgwick filter (accel + gyro)
//  • Yaw (heading): pure Z‑gyro integration  → **relative yaw**
//                   re‑zero at each arm / on RC switch if desired.
//--------------------------------------------------------------------
#include "IMU.h"
#include <Wire.h>

// ──────────────────────────────────────────────────────────────
// MPU‑6050 register definitions (minimal set we need)
//----------------------------------------------------------------
#define MPU_ADDR       0x68
#define REG_PWR_MGMT1  0x6B
#define REG_CONFIG     0x1A
#define REG_GYRO_CFG   0x1B
#define REG_ACCEL_CFG  0x1C
#define REG_ACCEL_DATA 0x3B   // first of 6 bytes

// 131 LSB per °/s  (±250°/s range)
const float GYRO_SCALE = 131.0f;
const float ACC_SCALE  = 16384.0f;   // LSB per g  (±2 g range)

static float rollOffset  = 0.0f;
static float pitchOffset = 0.0f;

// ──────────────────────────────────────────────────────────────
// Globals declared in IMU.h
//----------------------------------------------------------------
float roll  = 0.0f;
float pitch = 0.0f;
float yaw   = 0.0f;   // ← relative (integrated) yaw
float currentYawRate = 0.0f; // Definition for the extern variable

// Internal state
static Madgwick filter;
float gyroZBias = 0.0f;      // °/s
static unsigned long lastYawMicros = 0;

//----------------------------------------------------------------
// I‑O helpers
//----------------------------------------------------------------
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
    Wire.requestFrom(MPU_ADDR, len, (uint8_t)true);
    for (uint8_t i = 0; i < len; ++i) buf[i] = Wire.read();
}

//----------------------------------------------------------------
void setupIMU() {
    Wire.begin();
    delay(100);

    // Wake up & set gyro/accel ranges
    writeReg(REG_PWR_MGMT1, 0x00);          // wake
    writeReg(REG_GYRO_CFG,   0x00);         // ±250 °/s
    writeReg(REG_ACCEL_CFG,  0x00);         // ±2 g

    // Madgwick expects sample freq in Hz
    filter.begin(100.0f);                  // 100 Hz update

    Serial.println("MPU-6050 initialised (100 Hz)");
}

//----------------------------------------------------------------
// Gyro‑bias calibration – call while perfectly still
//----------------------------------------------------------------
void calibrateGyroZBias() {
    const uint16_t N = 500;   // ~1 s @ 2 ms reads
    long sum = 0;
    uint8_t buf[2];
    for (uint16_t i = 0; i < N; ++i) {
        readBurst(0x47, buf, 2);           // GZ_H & GZ_L
        int16_t gzRaw = (int16_t)((buf[0] << 8) | buf[1]);
        sum += gzRaw;
        delay(2);
    }
    gyroZBias   = (sum / (float)N) / GYRO_SCALE;
    lastYawMicros = micros();              // reset integrator clock
    yaw = 0.0f;

    Serial.printf("Gyro Z‑bias: %.4f °/s\n", gyroZBias);
}

// Take offsets for roll/pitch & determine gyro Z bias while completely still
void calibrateIMUOffsets()
{
    Serial.println(F("Calibrating IMU (keep the drone absolutely still) …"));
    delay(2000);

    // ── roll & pitch zero ────────────────────────────────────────────────
    const uint16_t N = 200;
    float rSum = 0, pSum = 0;
    for (uint16_t i = 0; i < N; ++i) {
        updateIMU();
        rSum += roll;
        pSum += pitch;
        delay(5);
    }

    rollOffset  = rSum / N;
    pitchOffset = pSum / N;
    Serial.printf("Roll offset:  %.2f°\n", rollOffset);
    Serial.printf("Pitch offset: %.2f°\n", pitchOffset);
    // ── gyro Z bias ───────────────────────────────────────────────────────
    calibrateGyroZBias();
}

//----------------------------------------------------------------
void updateIMU() {
    // ─── Read accel & gyro in one 14‑byte burst ───────────────
    uint8_t raw[14];
    readBurst(REG_ACCEL_DATA, raw, 14);

    int16_t ax = (int16_t)((raw[0]  << 8) | raw[1]);
    int16_t ay = (int16_t)((raw[2]  << 8) | raw[3]);
    int16_t az = (int16_t)((raw[4]  << 8) | raw[5]);
    int16_t gx = (int16_t)((raw[8]  << 8) | raw[9]);
    int16_t gy = (int16_t)((raw[10] << 8) | raw[11]);
    int16_t gz = (int16_t)((raw[12] << 8) | raw[13]);

    // ─── Convert to physical units ────────────────────────────
    float ax_g = ax / ACC_SCALE;
    float ay_g = ay / ACC_SCALE;
    float az_g = az / ACC_SCALE;
    float gx_dps = gx / GYRO_SCALE;
    float gy_dps = gy / GYRO_SCALE;
    float gz_dps = gz / GYRO_SCALE;

    // Update current yaw rate (bias-corrected)
    currentYawRate = gz_dps - gyroZBias;

    // ─── Integrate yaw (bias‑corrected) ───────────────────────
    unsigned long now = micros();
    if (lastYawMicros == 0) lastYawMicros = now;   // first call guard
    float dt = (now - lastYawMicros) * 1e-6f;      // seconds
    lastYawMicros = now;

    yaw += currentYawRate * dt;
    if (yaw >  180.0f) yaw -= 360.0f;
    if (yaw < -180.0f) yaw += 360.0f;

    // ─── Update Madgwick (IMU variant: no mag) ────────────────
    filter.updateIMU(gx_dps * DEG_TO_RAD,
                     gy_dps * DEG_TO_RAD,
                     gz_dps * DEG_TO_RAD,
                     ax_g, ay_g, az_g);

    // ─── Raw orientation from filter ──────────────────────────
    float rawRoll  = filter.getRoll();
    float rawPitch = filter.getPitch();

    // ─── Low‑pass on roll/pitch only (tunable) ────────────────
    static float fRoll  = 0.0f;
    static float fPitch = 0.0f;
    const float alpha = 0.40f;            // 0 = none, 1 = no smoothing

    fRoll  = alpha * rawRoll  + (1.0f - alpha) * fRoll;
    fPitch = alpha * rawPitch + (1.0f - alpha) * fPitch;

    // Apply offsets to roll and pitch
    roll  = fRoll  - rollOffset;
    pitch = fPitch - pitchOffset;

    // ─── Debug (comment out in flight) ────────────────────────
    // Serial.printf("R %.2f  P %.2f  Y %.2f\n", roll, pitch, yaw);
}
