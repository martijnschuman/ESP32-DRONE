// include/config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "status.h"

// ESPNow
#define MAIN_MAC_ADDRESS {0xD0, 0xEF, 0x76, 0x34, 0x97, 0x44}
#define CAMERA_MAC_ADDRESS {0xA0, 0xA3, 0xB3, 0xEC, 0xEC, 0x54}

enum CommandStatusEnum { // Remove the typedef
    KEEP_ALIVE = 0x01,
    TAKE_PICTURE = 0x02,
    READY = 0x03,
    FILE_SAVED = 0x04,
    SD_INIT_ERROR = 0x05,
    SD_SAVE_ERROR = 0x06,
    CAMERA_INIT_ERROR = 0x07,
    CAMERA_TAKE_ERROR = 0x08,
    ESP_NOW_INIT_ERROR = 0x09,
    ESP_NOW_ADD_PEER_ERROR = 0x0A,
    ESP_NOW_SEND_ERROR = 0x0B,
};

// Ensure CommandType is consistent
struct CommandPacket {
    CommandStatusEnum command;
};

extern CommandStatusEnum status;

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
