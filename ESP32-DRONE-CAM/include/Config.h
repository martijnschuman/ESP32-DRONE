// include/config.h
#ifndef CONFIG_H
#define CONFIG_H

#include "global.h"
#include <Arduino.h>
#include "status.h"

#define EEPROM_SIZE 64                          // Allocate 64 bytes for EEPROM
#define PICTURE_NUMBER_ADDR 0                   // Starting address for pictureNumber

// ESPNow
#define REMOTE_MAC_ADDRESS {0xCC, 0xDB, 0xA7, 0x3E, 0x66, 0x84}

// Status LED
#define STATUS_LED_PIN 33                       // Pin used to show the status of the system
#define FILE_SAVE_BLINK_COUNT 4                 // Number of times the LED blinks after saving a file

// Time intervals for blinking patterns
#define SYSTEM_ERROR_LED_ON_DURATION 150
#define SYSTEM_ERROR_LED_OFF_DURATION 50

#define ESP_NOW_ERROR_LED_ON_DURATION 500
#define ESP_NOW_ERROR_LED_OFF_DURATION 50

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#endif
