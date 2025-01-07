// include/Joystick.h

#ifndef Joystick_H
#define Joystick_H

#include "config.h"
#include "display.h"
#include "buttons.h"
#include "status.h"
#include "ADC.h"

extern bool leftCalibrated;
extern bool rightCalibrated;

void setupJoysticks();

bool getLeftJoystickButton();
bool getRightJoystickButton();

int getLeftJoystickX();
int getLeftJoystickY();
int getRightJoystickX();
int getRightJoystickY();
int transferJoystickValue(int value, int measuredMinValue, int measuredMaxValue, int centerValue, bool isYAxis);

void startCalibrateJoysticks();
bool calibrateSingleJoystick(float &minX, float &maxX, float &minY, float &maxY, float &centerX, float &centerY, int pinX, int pinY, const char* joystickName);

#endif