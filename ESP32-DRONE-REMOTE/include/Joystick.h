// include/Joystick.h

#ifndef Joystick_H
#define Joystick_H

#include <Arduino.h>
#include "Config.h"

void setupJoysticks();

bool checkStartCalibrationButton();
void calibrateLeftJoystick();
void calibrateRightJoystick();

int transferredLeftJoystickReadX();
int transferredLeftJoystickReadY();
int transferredRightJoystickReadX();
int transferredRightJoystickReadY();

bool getLeftCalibrated();
bool getRightCalibrated();

#endif // Joystick_H