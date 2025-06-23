// powerMonitor.cpp

#include "powerMonitor.h"

Adafruit_INA219 ina219;

float current;
float busVoltage;
float shuntVoltage;

void setupCurrentMonitor() {
    if (!ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
        droneState.status = ADC_ERROR;
    }
}

float readCurrent() {
    return ina219.getCurrent_mA();
}

float readBusVoltage() {
    return ina219.getBusVoltage_V();
}

float readShuntVoltage() {
    return ina219.getShuntVoltage_mV();
}

void updatePowerReadings() {
    current = readCurrent();
    busVoltage = readBusVoltage();
    shuntVoltage = readShuntVoltage();
}

bool isBatteryLow() {
    return busVoltage < BATTERY_MIN_VOLTAGE;
}
