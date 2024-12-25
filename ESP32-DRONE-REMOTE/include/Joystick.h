// include/Joystick.h

#ifndef Joystick_H
#define Joystick_H

#include <Arduino.h>
#include "Config.h"

void setupJoysticks();

bool checkStartCalibrationButton();
void calibrateLeftJoystick();

int transferredLeftJoystickReadX();
int transferredLeftJoystickReadY();

bool getLeftCalibrated();

#endif // Joystick_H