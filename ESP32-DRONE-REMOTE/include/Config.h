// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "global.h"

// Transmission interval
#define TRANSMISSION_INTERVAL 500						// Interval for sending telemetry data
#define FIRST_CONNECTION_INTERVAL 5000					// Interval for first connection attempt
#define CAMERA_CONNECTION_INTERVAL 5000					// Interval for camera first connection attempt
#define CAMERA_PICTURE_INTERVAL 3000					// Interval for taking pictures

// Button pins  
#define BUTTON_OK 15									// 
#define BUTTON_ALT 14									// 
#define DEBOUNCE_DELAY 50								// Debounce delay for buttons

// ADC
#define ADC_ADDRESS 0x48								// ADS1115 I2C address
#define ADC_GAIN GAIN_ONE								// ADC gain

// Joystick
#define JOYSTICK_LEFT_X_CHANNEL 2						// Left joystick X-axis channel
#define JOYSTICK_LEFT_Y_CHANNEL 3						// Left joystick Y-axis channel
#define JOYSTICK_RIGHT_X_CHANNEL 0						// Right joystick X-axis channel
#define JOYSTICK_RIGHT_Y_CHANNEL 1						// Right joystick Y-axis channel
#define JOYSTICK_LEFT_BUTTON 12							// Left joystick button pin
#define JOYSTICK_RIGHT_BUTTON 13						// Right joystick button pin

// Joystick calibration
#define JOYSTICK_CALIBRATION_DELAY 3000
#define JOYSTICK_CENTER_TOLERANCE 5

// LCD
#define X_PIXELS 128								    // Number of pixels in the X-axis
#define Y_PIXELS 64										// Number of pixels in the Y-axis

// Status LEDs
#define STATUS_RED_LED 32
#define STATUS_GREEN_LED 23

// ESPNow
#define DRONE_MAC_ADDRESS {0xD0, 0xEF, 0x76, 0x34, 0x97, 0x44}
#define CAMERA_MAC_ADDRESS {0xA0, 0xA3, 0xB3, 0xEC, 0xEC, 0x54}

#endif // CONFIG_H