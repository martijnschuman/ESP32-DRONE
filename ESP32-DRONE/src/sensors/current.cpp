//current.cpp

#include "current.h"

Adafruit_INA219 ina219;

float current;
float voltage;

void setupCurrentMonitor() {
    if (!ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
        droneState.status = ADC_ERROR;
        return;
    }
}

float readCurrent() {
    return ina219.getCurrent_mA();
}

float readVoltage() {
    return ina219.getBusVoltage_V();
}

float updateCurrent() {
    return current = readCurrent();
}

float updateVoltage() {
    return voltage = readVoltage();
}
