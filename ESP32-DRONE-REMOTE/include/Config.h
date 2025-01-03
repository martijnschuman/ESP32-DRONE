// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "global.h"

// Transmission interval
#define TRANSMISSION_INTERVAL 50
#define FIRST_CONNECTION_INTERVAL 1000

// Joystick pins
#define LEFT_VRX 34
#define LEFT_VRY 35
#define RIGHT_VRX 27
#define RIGHT_VRY 26

// Button pins  
#define BUTTON_OK 23
#define BUTTON_ALT 15
#define DEBOUNCE_DELAY 75

// Joystick calibration
#define JOYSTICK_CALIBRATION_DELAY 3000
#define JOYSTICK_CENTER_TOLERANCE 5

// Status LEDs
#define STATUS_RED_LED 19
#define STATUS_GREEN_LED 18

// ESPNow
#define DRONE_MAC_ADDRESS {0xD0, 0xEF, 0x76, 0x34, 0x97, 0x44}
#define REMOTE_MAC_ADDRESS {0xCC, 0xDB, 0xA7, 0x3E, 0x66, 0x84}
#define CAMERA_MAC_ADDRESS {0xA0, 0xA3, 0xB3, 0xEC, 0x3C, 0x54}

#define CAMERA_KEEP_ALIVE_INTERVAL 5000
#define DRONE_KEEP_ALIVE_INTERVAL 1000

#endif // CONFIG_H