// include/Joystick.h

#ifndef Joystick_H
#define Joystick_H

#include "config.h"
#include "LCD.h"
#include "buttons.h"
#include "status.h"

extern bool leftCalibrated;
extern bool rightCalibrated;

void setupJoysticks();

int getLeftJoystickX();
int getLeftJoystickY();
int getRightJoystickX();
int getRightJoystickY();
int transferJoystickValue(int value, int measuredMinValue, int measuredMaxValue, int centerValue, bool isYAxis);

void startCalibrateJoysticks();
bool calibrateSingleJoystick(int &minX, int &maxX, int &minY, int &maxY, int &centerX, int &centerY, int pinX, int pinY, const char* joystickName);

#endif