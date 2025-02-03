// include/battery.h

#ifndef BATTERY_H
#define BATTERY_H

#include "config.h"
#include "status.h"

void setupBatteryMonitor();
bool isBatteryVoltageGoodForSetup();
bool isBatteryVoltageGoodForFlight();

#endif