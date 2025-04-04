// include/Joystick.h

#ifndef LCD_H
#define LCD_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1305.h>
#include "config.h"

extern Adafruit_SSD1305 display;

void setupDisplay();
void displayStartCalibration();
void displayCalibration(int stage, const char* joystickName);
void displayStartDroneConnection();
void displayStartCameraConnection();
void displayReadyToFly();
void displayArmingDrone();
void displayTelemetry();
void displayESPNowError();

#endif
