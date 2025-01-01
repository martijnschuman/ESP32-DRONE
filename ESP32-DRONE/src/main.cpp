// src/main.cpp
#include <Arduino.h>
#include "GPS.h"
#include "serial.h"
#include "LIDAR.h"
#include "IMU.h"
#include "I2CMultiplexer.h"
#include "ESPNow.h"
#include "status.h"
#include "telemetry.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

unsigned long currentMillis = 0;
unsigned long lastIMUUpdate = 0;
unsigned long lastLIDARUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastTransmitUpdate = 0;

void setup(void) {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

    setupSerial();
    setupStatusDisplay();
    displayClear();

    setupI2CMultiplexer();
    enableI2CChannel(MPU6050_CHANNEL);
    enableI2CChannel(LIDAR_CHANNEL);

    setupIMU();
    setupLIDAR();
    setupGPS();

    setupESPNow();

    Serial.println("Setup complete.");
}

void loop() {
    reportStatus();

    currentMillis = millis();

    // Update IMU
    if (currentMillis - lastIMUUpdate >= IMU_INTERVAL) {
        lastIMUUpdate = currentMillis;
        updateIMU(); // Function for averaging IMU data
    }

    // Update LIDAR
    if (currentMillis - lastLIDARUpdate >= LIDAR_INTERVAL) {
        lastLIDARUpdate = currentMillis;
        updateLIDAR(); // Function for averaging LIDAR data
    }

    // Update Display
    if (currentMillis - lastDisplayUpdate >= SERIAL_DEBUG_INTERVAL) {
        lastDisplayUpdate = currentMillis;
        printTelemetry(); // Display the averaged sensor data
    }

    // Transmit telemetry via ESP-NOW
    if (currentMillis - lastTransmitUpdate >= TRANSMIT_INTERVAL) {
        lastTransmitUpdate = currentMillis;
        transmitTelemetry(); // Send data to remote controller
    }
}