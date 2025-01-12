// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "global.h"

// Transmission interval
#define TRANSMISSION_INTERVAL 500						// Interval for sending telemetry data
#define FIRST_CONNECTION_INTERVAL 5000					// Interval for first connection attempt

// Button pins  
#define BUTTON_OK 23									// 
#define BUTTON_ALT 15									// 
#define DEBOUNCE_DELAY 50								// Debounce delay for buttons

// I2C Multiplexer
#define TCA9548A_ADDRESS 0x75							// I2C address of the multiplexer
#define ADC_CHANNEL 1									// Multiplexer channel for the MPU6050
#define DISPLAY_CHANNEL 0 								// Multiplexer channel for the LIDAR

// ADC
#define ADC_ADDRESS 0x48								// ADS1115 I2C address
#define ADC_GAIN GAIN_ONE								// ADC gain

// Joystick
#define JOYSTICK_LEFT_X_CHANNEL 0						// Left joystick X-axis channel
#define JOYSTICK_LEFT_Y_CHANNEL 1						// Left joystick Y-axis channel
#define JOYSTICK_RIGHT_X_CHANNEL 2						// Right joystick X-axis channel
#define JOYSTICK_RIGHT_Y_CHANNEL 3						// Right joystick Y-axis channel
#define JOYSTICK_LEFT_BUTTON 12							// Left joystick button pin
#define JOYSTICK_RIGHT_BUTTON 13						// Right joystick button pin

// Joystick calibration
#define JOYSTICK_CALIBRATION_DELAY 3000
#define JOYSTICK_CENTER_TOLERANCE 5

// LCD
#define X_PIXELS 128								    // Number of pixels in the X-axis
#define Y_PIXELS 64										// Number of pixels in the Y-axis

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