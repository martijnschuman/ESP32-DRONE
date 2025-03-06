// src/main.cpp
#include <Arduino.h>
#include "config.h"
#include "global.h"
#include "status.h"
#include "serial.h"
#include "battery.h"
#include "GPS.h"
#include "echo.h"
#include "IMU.h"
#include "ESPNow.h"
#include "ESC.h"
#include "powerMonitor.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

unsigned long currentMillis = 0;
unsigned long lastIMUUpdate = 0;
unsigned long lastHEIGHTUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastTransmitUpdate = 0;
unsigned long lastConnectionCheck = 0;
unsigned long lastBatteryMonitor = 0;
unsigned long lastPowerMonitor = 0;

bool isConnectedToRemote = false;

bool isESCOneArmed = false;
bool isESCTwoArmed = true;
bool isESCThreeArmed = false;
bool isESCFourArmed = true;

DroneState droneState;

ControlPacket controlPacket = {};
DroneStatePacket droneStatePacket = {};

void setup(void) {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

    setupSerial();
    setupStatusDisplay();
    displayClear();

    // setupBatteryMonitor();    
    // if (!getBatteryStatus()) {
    //     return;
    // }

    setupCurrentMonitor();

    setupIMU();
    setupEcho();
    // setupGPS();

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
    // if (currentMillis - lastBatteryMonitor >= BATTERY_MONITOR_INTERVAL) {
    //     lastBatteryMonitor = currentMillis;
    //     if (!getBatteryStatus()) {
    //         return;
    //     }
    // }

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
        Serial.println("Starting ESC arming...");
        delay(4000);
        if (!isESCOneArmed) {
            setupESC(ESCOne, ESC_ONE_PIN);
            delay(1000);
            armESC(ESCOne);
            isESCOneArmed = true;
        }
        if (!isESCThreeArmed) {
            setupESC(ESCThree, ESC_THREE_PIN);
            delay(1000);
            armESC(ESCThree);
            isESCThreeArmed = true;
        }

        if(isESCOneArmed && isESCThreeArmed) {
            Serial.println("ESCs armed.");
            sendDroneFlightReady();
        }
    }

    if (getStatus() == READY && getFlightMode() == MANUAL) {
        // Update IMU
        if (currentMillis - lastIMUUpdate >= IMU_INTERVAL) {
            lastIMUUpdate = currentMillis;
            updateIMU();
        }

        // Update ECHO
        if (currentMillis - lastHEIGHTUpdate >= ECHO_INTERVAL) {
            lastHEIGHTUpdate = currentMillis;
            updateHeight();
        }

        // Transmit telemetry to the remote
        if (currentMillis - lastTransmitUpdate >= TRANSMIT_INTERVAL) {
            lastTransmitUpdate = currentMillis;
            sendTelemetry();
        }

        // Update powerMonitor
        if (currentMillis - lastPowerMonitor >= POWER_MONITOR_INTERVAL) {
            lastPowerMonitor = currentMillis;
            updateCurrent();
            updateBusVoltage();
            updateShuntVoltage();
        }

        // Apply throttle to ESCs
        setESC(ESC_ONE_PIN, controlPacket.throttle);
        // setESC(ESC_TWO_PIN, controlPacket.throttle);
        setESC(ESC_THREE_PIN, controlPacket.throttle);
        // setESC(ESC_FOUR_PIN, controlPacket.throttle);
    }
}
