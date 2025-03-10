// src/sensors/IMU.cpp
#include "IMU.h"

Adafruit_MPU6050 mpu;

// Global variables for calibration
float gyroXOffset = 0, gyroYOffset = 0, gyroZOffset = 0;
float accelXOffset = 0, accelYOffset = 0, accelZOffset = 0;

// Global variables for IMU data
float accX = 0.0f, accY = 0.0f, accZ = 0.0f;
float gyroX = 0.0f, gyroY = 0.0f, gyroZ = 0.0f;

// Global variables for orientation angles
float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;

void readMPU6050(sensors_event_t *a, sensors_event_t *g, sensors_event_t *temp) {
    mpu.getEvent(a, g, temp);
}

void calibrateGyro() {
    Serial.println("Calibrating gyro...");
    
    for (int i = 0; i < IMU_SETUP_CALIBRATION_COUNT; i++) {
        sensors_event_t a, g, temp;
        readMPU6050(&a, &g, &temp);
        
        gyroXOffset += g.gyro.x;
        gyroYOffset += g.gyro.y;
        gyroZOffset += g.gyro.z;
        delay(5);
    }

    // Calculate average offsets
    gyroXOffset /= IMU_SETUP_CALIBRATION_COUNT;
    gyroYOffset /= IMU_SETUP_CALIBRATION_COUNT;
    gyroZOffset /= IMU_SETUP_CALIBRATION_COUNT;

    Serial.print("Gyro Offsets: ");
    Serial.print(gyroXOffset);
    Serial.print(", ");
    Serial.print(gyroYOffset);
    Serial.print(", ");
    Serial.println(gyroZOffset);
}

void setupIMU() {
    if (!initializeMPU6050()) {
        Serial.println("MPU6050 initialization failed!");
        throwError(IMU_ERROR);
        while (1) { delay(10); }
    } else {
        Serial.println("MPU6050 initialized!");
    }

    calibrateGyro();
    Serial.println("IMU setup complete.");
}

bool initializeMPU6050() {
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        throwError(IMU_ERROR);
        return false;
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    Serial.println("IMU accelerometer range set");

    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.println("IMU gyro range set");

    mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
    Serial.println("IMU filter bandwidth set");

    return true;
}

void updateIMU() {
    sensors_event_t a, g, tempEvent;
    readMPU6050(&a, &g, &tempEvent);

    // Time difference (dt) in seconds
    static unsigned long lastTime = 0;
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // Update accelerometer data
    accX = a.acceleration.x - accelXOffset;
    accY = a.acceleration.y - accelYOffset;
    accZ = a.acceleration.z;

    // Update gyroscope data
    gyroX = g.gyro.x - gyroXOffset;
    gyroY = g.gyro.y - gyroYOffset;
    gyroZ = g.gyro.z - gyroZOffset;

    // Calculate roll and pitch angles
    roll = atan2(accY, accZ) * RAD_TO_DEG;
    pitch = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;

    // Integrate gyroscope Z-axis data to calculate yaw
    yaw += gyroZ * dt; // Ensure gyroZ is in degrees per second
}
