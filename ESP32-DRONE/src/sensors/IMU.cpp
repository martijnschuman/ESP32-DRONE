// src/sensors/IMU.cpp
#include "IMU.h"

Adafruit_MPU6050 mpu;

// Circular buffer for IMU data
float accXBuffer[BUFFER_SIZE], accYBuffer[BUFFER_SIZE], accZBuffer[BUFFER_SIZE];
float gyroXBuffer[BUFFER_SIZE], gyroYBuffer[BUFFER_SIZE], gyroZBuffer[BUFFER_SIZE];
float tempBuffer[BUFFER_SIZE];

// Index for circular buffer
int imuIndex = 0;
float accXSum = 0, accYSum = 0, accZSum = 0;
float gyroXSum = 0, gyroYSum = 0, gyroZSum = 0;
float tempSum = 0;

// Global variables for IMU data
float accX = 0.0f, accY = 0.0f, accZ = 0.0f;
float gyroX = 0.0f, gyroY = 0.0f, gyroZ = 0.0f;
float temp = 0.0f;

// Initialize MPU6050
void setupIMU() {
    if (!initializeMPU6050()) {
        Serial.println("MPU6050 initialization failed!");
        throwError(IMU_ERROR);
        while (1) {
            delay(10);
        }
    }
    else {
        Serial.println("MPU6050 initialization successful!");
    }
}

bool initializeMPU6050() {
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        throwError(IMU_ERROR);
        return false;
    }
    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }

    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }

    return true;
}

void readMPU6050(sensors_event_t *a, sensors_event_t *g, sensors_event_t *temp) {
    mpu.getEvent(a, g, temp);
}

float getCalibratedTemperature(float temp) {
    return temp + IMU_TEMP_CALIBRATION;
}

void updateIMU() {
    sensors_event_t a, g, tempEvent;
    readMPU6050(&a, &g, &tempEvent);

    // Update acceleration rolling averages
    accXSum -= accXBuffer[imuIndex];
    accXBuffer[imuIndex] = a.acceleration.x;
    accXSum += accXBuffer[imuIndex];

    accYSum -= accYBuffer[imuIndex];
    accYBuffer[imuIndex] = a.acceleration.y;
    accYSum += accYBuffer[imuIndex];

    accZSum -= accZBuffer[imuIndex];
    accZBuffer[imuIndex] = a.acceleration.z;
    accZSum += accZBuffer[imuIndex];

    accX = accXSum / BUFFER_SIZE;
    accY = accYSum / BUFFER_SIZE;
    accZ = accZSum / BUFFER_SIZE;

    // Update gyro rolling averages
    gyroXSum -= gyroXBuffer[imuIndex];
    gyroXBuffer[imuIndex] = g.gyro.x;
    gyroXSum += gyroXBuffer[imuIndex];

    gyroYSum -= gyroYBuffer[imuIndex];
    gyroYBuffer[imuIndex] = g.gyro.y;
    gyroYSum += gyroYBuffer[imuIndex];

    gyroZSum -= gyroZBuffer[imuIndex];
    gyroZBuffer[imuIndex] = g.gyro.z;
    gyroZSum += gyroZBuffer[imuIndex];

    gyroX = gyroXSum / BUFFER_SIZE;
    gyroY = gyroYSum / BUFFER_SIZE;
    gyroZ = gyroZSum / BUFFER_SIZE;

    // Update temperature rolling average
    tempSum -= tempBuffer[imuIndex];
    tempBuffer[imuIndex] = getCalibratedTemperature(tempEvent.temperature);
    tempSum += tempBuffer[imuIndex];

    temp = tempSum / BUFFER_SIZE;

    // Advance index for circular buffer
    imuIndex = (imuIndex + 1) % BUFFER_SIZE;
}
