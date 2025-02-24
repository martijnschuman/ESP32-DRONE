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
#include "current.h"
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

    Serial.println("Battery voltage is critical: " + String(readBatteryVoltage()) + "V");
    Serial.println("Raw ADC value: " + String(analogRead(BATTERY_VOLTAGE_PIN)));

    setupCurrentMonitor();

    setupIMU();
    setupLIDAR();
    // setupGPS();

    setupESPNow();

    Serial.println("Setup complete.");
    setStatus(START_CONNECTION);
}

// void loop(){
//     Serial.print("Battery voltage: ");
//     Serial.println(readBatteryVoltage());
//     Serial.print("Current: ");
//     Serial.println(readCurrent());
//     Serial.print("Voltage: ");
//     Serial.println(readVoltage());
//     delay(1000);
// }

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
        Serial.println("Starting ESC arming...");
        delay(4000);
        if (!isESCOneArmed) {
            setupESC(ESCOne, ESC_ONE_PIN);
            delay(1000);
            armESC(ESCOne);
            isESCOneArmed = true;
        }
        sendDroneFlightReady();
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
