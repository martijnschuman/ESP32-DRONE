// src/main.cpp
#include <Arduino.h>
#include "config.h"
#include "global.h"
#include "status.h"
#include "serial.h"
#include "echo.h"
#include "IMU.h"
#include "ESPNow.h"
#include "ESC.h"
#include "powerMonitor.h"
#include "PIDControl.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

unsigned long currentMillis = 0;
unsigned long lastStatusReport = 0;
unsigned long lastIMUUpdate = 0;
unsigned long lastHeightUpdate = 0;
unsigned long lastTransmitUpdate = 0;
unsigned long lastBatteryMonitor = 0;
unsigned long lastPowerMonitor = 0;
unsigned long lastPIDUpdate = 0;

bool hasBeenThrottled = false;
bool isConnectedToRemote = false;

DroneState droneState;

ControlPacket controlPacket = {};
DroneStatePacket droneStatePacket = {};

void setup(void) {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

    setupSerial();
    setupStatusDisplay();
    displayClear();

    setupCurrentMonitor();

    setupIMU();
    calibrateIMUOffsets();
    setupEcho();

    setupESPNow();

    Serial.println("Setup complete.");
    setStatus(START_CONNECTION);
}

void loop() {
    currentMillis = millis();

    // Report status
    if (currentMillis - lastStatusReport >= STATUS_REPORT_INTERVAL) {
        lastStatusReport = currentMillis;
        reportStatus();
    }

    // Check battery voltage
    if (currentMillis - lastBatteryMonitor >= POWER_MONITOR_INTERVAL) {
        lastBatteryMonitor = currentMillis;
        updatePowerReadings();

        if (isBatteryLow()) {
            Serial.println("Battery low. Switching to emergency descent mode.");
            setFlightMode(EMERGENCY_DECENT);
            setStatus(EMERGENCY);
            return;
        }
    }

    // Handle emergency descent if active
    if ((getFlightMode() == EMERGENCY_DECENT && getStatus() == EMERGENCY)) {
        emergencyDescent();
        return;
    }

    // Check if the remote is connected
    if (!isConnectedToRemote) {
        return;
    }

    // Arm ESCs
    if (getStatus() == READY && getFlightMode() == GROUND) {
        bool ESCsArmed = armAllESCs();

        if(ESCsArmed) {
            Serial.println("ESCs armed.");
            sendDroneFlightReady();
        }
    }

    if (getStatus() == READY && getFlightMode() == MANUAL) {
        // Safety check: if no message has been received within CONNECTION_TIMEOUT, switch to emergency descent
        if ((currentMillis - lastCommunicationTime >= CONNECTION_TIMEOUT) && hasBeenThrottled) {
            Serial.println("Communication lost. Switching to emergency descent mode.");
            setFlightMode(EMERGENCY_DECENT);
            setStatus(EMERGENCY);
            isConnectedToRemote = false;
            return;
        }

        // Update IMU
        if (currentMillis - lastIMUUpdate >= IMU_INTERVAL) {
            lastIMUUpdate = currentMillis;
            updateIMU();
        }

        // Update ECHO
        if (currentMillis - lastHeightUpdate >= ECHO_INTERVAL) {
            lastHeightUpdate = currentMillis;
            updateHeight();
        }

        // Transmit telemetry to the remote
        if (currentMillis - lastTransmitUpdate >= TRANSMIT_INTERVAL) {
            lastTransmitUpdate = currentMillis;
            sendTelemetry();
        }

        // Apply throttle to ESCs
        // setESC(ESC_ONE_PIN, controlPacket.throttle);
        // setESC(ESC_TWO_PIN, controlPacket.throttle);
        // setESC(ESC_THREE_PIN, controlPacket.throttle);
        // setESC(ESC_FOUR_PIN, controlPacket.throttle);

        if (currentMillis - lastPIDUpdate >= PID_UPDATE_INTERVAL) {
            lastPIDUpdate = currentMillis;
            updatePIDControl();
            checkSerialCommands();
        }
    }
}
