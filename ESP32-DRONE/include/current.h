//include/current.h

#ifndef CURRENT_H
#define CURRENT_H

#include "config.h"
#include <Adafruit_INA219.h>

extern float current;
extern float voltage;

void setupCurrentMonitor();
float readCurrent();
float readVoltage();
float updateCurrent();
float updateVoltage();

#endif
