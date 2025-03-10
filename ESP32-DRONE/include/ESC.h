// include/ESC.h

#ifndef ESC_H
#define ESC_H

#include "config.h"
#include "IMU.h"
#include <ESP32Servo.h>

extern Servo ESCOne;
extern Servo ESCTwo;
extern Servo ESCThree;
extern Servo ESCFour;

extern bool isESCOneInitialized;
extern bool isESCTwoInitialized;
extern bool isESCThreeInitialized;
extern bool isESCFourInitialized;

extern bool isESCOneArmed;
extern bool isESCTwoArmed;
extern bool isESCThreeArmed;
extern bool isESCFourArmed;

void setupESC(Servo& ESC, int ESC_PIN);
void armESC(Servo& ESC);
bool armAllESCs();
void setESC(int ESCPin, float amount);
void applyMotorAdjustments(float rollAdjust, float pitchAdjust, float yawAdjust);

#endif