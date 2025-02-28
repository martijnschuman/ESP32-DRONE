// battery.cpp

#include "battery.h"

float batteryVoltage;

// Set up battery monitoring
void setupBatteryMonitor() {
    analogReadResolution(12);
    pinMode(BATTERY_VOLTAGE_PIN, INPUT);
}

const float r1 = 3000.0;
const float r2 = 220.0;
// Read battery voltage
float readBatteryVoltage() {
    return analogRead(BATTERY_VOLTAGE_PIN) * (3.65 / 4095.0) * ((r1 + r2) / r2);
}

// Check if battery voltage is good for setup
bool getBatteryStatus() {
    float calibratedVoltage = 0.0;
    for (int i = 0; i < BATTERY_SETUP_READINGS; i++) {
        calibratedVoltage += readBatteryVoltage();
        delay(BATTERY_SETUP_INTERVAL); // Small delay between readings
    }
    calibratedVoltage /= BATTERY_SETUP_READINGS;

    if (calibratedVoltage <= BATTERY_CRITICAL_VOLTAGE) {
        //Serial.println("Battery voltage is critical: " + String(calibratedVoltage) + "V");
        //Serial.println("Raw ADC value: " + String(analogRead(BATTERY_VOLTAGE_PIN)));
        setStatus(BATTERY_CRITICAL);
        return false;
    } else if (calibratedVoltage <= BATTERY_LOW_VOLTAGE) {
        //Serial.println("Battery voltage is critical: " + String(calibratedVoltage) + "V");
        //Serial.println("Raw ADC value: " + String(analogRead(BATTERY_VOLTAGE_PIN)));
        setStatus(BATTERY_LOW);
        return false;
    }

    batteryVoltage = calibratedVoltage;

    return true;
}

