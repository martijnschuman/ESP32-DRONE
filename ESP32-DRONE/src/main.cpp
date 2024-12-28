// src/main.cpp
#include <Arduino.h>
#include "TinyGPS++.h"
#include "HardwareSerial.h"
#include "GPS.h"
#include "Serial.h"
#include "LIDAR.h"
#include "IMU.h"
#include "I2CMultiplexer.h"

void setup(void) {
    setupSerial();
    setupI2CMultiplexer();

    enableI2CChannel(MPU6050_CHANNEL);
    enableI2CChannel(LIDAR_CHANNEL);

    setupIMU();
    // setupGPS();
    setupLIDAR();

    Serial.println("Setup done");
}

void loop() {
    sensors_event_t a, g, temp;
    readMPU6050(&a, &g, &temp);
    printMPU6050Data(&a, &g, &temp);

    // displayGPSData();

    measureHight();

    delay(1000);
}