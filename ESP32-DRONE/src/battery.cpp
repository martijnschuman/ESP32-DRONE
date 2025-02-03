// battery.cpp

#include "battery.h"

// Set up battery monitoring
void setupBatteryMonitor() {
    analogReadResolution(12);
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
}

// Read battery voltage
float readBatteryVoltage() {
    return analogRead(BATTERY_VOLTAGE_PIN) * (3.6 / 4095.0) * BATTERY_DIVIDER_RATIO;
}

// Check if battery voltage is good for setup
bool isBatteryVoltageGoodForSetup() {
    float batteryVoltage = 0.0;
    for (int i = 0; i < BATTERY_SETUP_READINGS; i++) {
        batteryVoltage += readBatteryVoltage();
        delay(BATTERY_SETUP_INTERVAL); // Small delay between readings
    }
    batteryVoltage /= BATTERY_SETUP_READINGS;

    if (batteryVoltage <= BATTERY_CRITICAL_VOLTAGE) {
        setStatus(BATTERY_CRITICAL);
        return false;
    } else if (batteryVoltage <= BATTERY_LOW_VOLTAGE) {
        setStatus(BATTERY_LOW);
        return false;
    }

    return true;
}

// Check if battery voltage is good for operation
bool isBatteryVoltageGoodForFlight() {
    float batteryVoltage = readBatteryVoltage();
    if (batteryVoltage <= BATTERY_CRITICAL_VOLTAGE) {
        setStatus(BATTERY_CRITICAL);
        return false;
    } else if (batteryVoltage <= BATTERY_LOW_VOLTAGE) {
        setStatus(BATTERY_LOW);
        return false;
    }

    return true;
}