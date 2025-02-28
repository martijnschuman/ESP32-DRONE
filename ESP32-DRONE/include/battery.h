// include/battery.h

#ifndef BATTERY_H
#define BATTERY_H

#include "config.h"
#include "status.h"
#include <SPI.h>

extern float batteryVoltage;

void setupBatteryMonitor();
float readBatteryVoltage();
bool getBatteryStatus();

#endif