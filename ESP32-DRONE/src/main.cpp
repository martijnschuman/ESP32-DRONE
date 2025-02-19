// src/main.cpp
#include <Arduino.h>
#include "config.h"
#include "global.h"
#include "status.h"
#include "serial.h"
#include "battery.h"
#include "GPS.h"
#include "LIDAR.h"
#include "IMU.h"
#include "ESPNow.h"
#include "telemetry.h"
#include "ESC.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

unsigned long currentMillis = 0;
unsigned long lastIMUUpdate = 0;
unsigned long lastLIDARUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastTransmitUpdate = 0;
unsigned long lastConnectionCheck = 0;
unsigned long lastBatteryMonitor = 0;

bool isConnectedToRemote = false;

bool isESCOneInitialized = false;
bool isESCTwoInitialized = true;
bool isESCThreeInitialized = true;
bool isESCFourInitialized = true;

bool isESCOneArmed = false;
bool isESCTwoArmed = true;
bool isESCThreeArmed = true;
bool isESCFourArmed = true;

DroneState droneState;

ControlPacket controlPacket = {};
DroneStatePacket droneStatePacket = {};

void setup(void) {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

    setupSerial();
    setupStatusDisplay();
    displayClear();

    setupBatteryMonitor();
    if (!isBatteryVoltageGoodForSetup()) {
        return;
    }

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
    static unsigned long lastBatteryMonitor = 0;

    currentMillis = millis();

    // Check battery voltage
    if (currentMillis - lastBatteryMonitor >= BATTERY_MONITOR_INTERVAL) {
        lastBatteryMonitor = currentMillis;
        if (!isBatteryVoltageGoodForFlight()) {
            return;
        }
    }

    // Report status
    if (currentMillis - lastStatusReport >= STATUS_REPORT_INTERVAL) {
        lastStatusReport = currentMillis;
        reportStatus();
    }

    // Check if the remote is connected
    if (!isConnectedToRemote) {
        return;
    }

    // Arm ESCs
    if (getStatus() == READY && getFlightMode() == GROUND) {
        if(isESCOneInitialized && isESCTwoInitialized && isESCThreeInitialized && isESCFourInitialized) {
            if (!isESCOneArmed) {
                armESC(ESCOne);
                isESCOneArmed = true;
            }
            // if (!isESCTwoArmed) {
            //     armESC(ESCTwo);
            //     isESCTwoArmed = true;
            // }
            // if (!isESCThreeArmed) {
            //     armESC(ESCThree);
            //     isESCThreeArmed = true;
            // }
            // if (!isESCFourArmed) {
            //     armESC(ESCFour);
            //     isESCFourArmed = true;
            // }

            sendDroneFlightReady();
        }
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

        // Transmit telemetry to the remote
        if (currentMillis - lastTransmitUpdate >= TRANSMIT_INTERVAL) {
            lastTransmitUpdate = currentMillis;
            sendTelemetry();
        }

        // Apply throttle to ESCs
        setESC(ESC_ONE_PIN, controlPacket.throttle);
        // setESC(ESC_TWO_PIN, controlPacket.throttle);
        // setESC(ESC_THREE_PIN, controlPacket.throttle);
        // setESC(ESC_FOUR_PIN, controlPacket.throttle);
    }
}
