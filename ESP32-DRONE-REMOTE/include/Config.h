// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

extern bool isDroneConnected;

#define LEFT_VRX 34
#define LEFT_VRY 35

#define RIGHT_VRX 27
#define RIGHT_VRY 26

#define BUTTON_OK 23
#define BUTTON_ALT 15
#define DEBOUNCE_DELAY 75

#define JOYSTICK_CALIBRATION_DELAY 3000
#define JOYSTICK_CENTER_TOLERANCE 5

#define CALIBRATION_RED_LED 19
#define CALIBRATION_GREEN_LED 18

#endif // CONFIG_H