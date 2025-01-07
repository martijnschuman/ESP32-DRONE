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

#endif
