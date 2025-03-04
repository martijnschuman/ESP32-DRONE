//powerMonitor.cpp

#include "powerMonitor.h"

Adafruit_INA219 ina219;

float current;
float busVoltage;
float shuntVoltage;

void setupCurrentMonitor() {
    if (!ina219.begin()) {
        //Serial.println("Failed to find INA219 chip");
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

float updateCurrent() {
    return current = readCurrent();
}

float updateBusVoltage() {
    return busVoltage= readBusVoltage();
}

float updateShuntVoltage() {
    return shuntVoltage = readShuntVoltage();
}
