// include/Joystick.h

#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "joystick.h"

#define LCD_ADDRESS 0x27                // I2C address of the LCD
#define LCD_COLUMNS 16                  // Number of columns on the LCD
#define LCD_ROWS 2                      // Number of rows on the LCD
#define LCD_REFRESH_INTERVAL 1500       // Interval for displaying text

void setupLCD();
void displayLCD(const char* message, int row, int column);
void clearLCD();
void clearRow(int row);

// Menus
void calibrationMenu();
void connectToDroneDisplay();
void connectedToDroneDisplay();
void connectToDroneFailedDisplay();
void joystickDebugDisplay();

#endif