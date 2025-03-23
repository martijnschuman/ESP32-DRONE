//include/current.h

#ifndef CURRENT_H
#define CURRENT_H

#include "config.h"
#include <Adafruit_INA219.h>

extern float current;
extern float busVoltage;
extern float shuntVoltage;

void setupCurrentMonitor();
void updatePowerReadings();
bool isBatteryLow();

#endif
