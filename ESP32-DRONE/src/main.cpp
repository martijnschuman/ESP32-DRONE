// src/main.cpp
#include <Arduino.h>
#include "config.h"
#include "global.h"
#include "status.h"
#include "serial.h"
#include "I2CMultiplexer.h"
#include "GPS.h"
#include "LIDAR.h"
#include "IMU.h"
#include "ESPNow.h"
#include "telemetry.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

unsigned long currentMillis = 0;
unsigned long lastIMUUpdate = 0;
unsigned long lastLIDARUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastTransmitUpdate = 0;
unsigned long lastConnectionCheck = 0;

bool isConnectedToRemote = false;

DroneState droneState;

ControlPacket controlPacket = {};
DroneStatePacket droneStatePacket = {};

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
    setStatus(START_CONNECTION);
}

void loop() {
    static unsigned long lastStatusReport = 0;
    static unsigned long lastConnectionTest = 0;

    currentMillis = millis();

    // Report status
    if (currentMillis - lastStatusReport >= STATUS_REPORT_INTERVAL) {
        lastStatusReport = currentMillis;
        reportStatus();
    }

    // Check if the remote is connected
    if (!isConnectedToRemote) {
        return;
    }

    if (getStatus() == READY && getFlightMode() == MANUAL) {
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
        // if (currentMillis - lastDisplayUpdate >= SERIAL_DEBUG_INTERVAL) {
        //     lastDisplayUpdate = currentMillis;
        //     printTelemetry(); // Display the averaged sensor data
        // }

        // Transmit telemetry to the remote
        // if (currentMillis - lastTransmitUpdate >= TRANSMIT_INTERVAL) {
        //     lastTransmitUpdate = currentMillis;
        //     sendTelemetry();
        // }
    }
}
