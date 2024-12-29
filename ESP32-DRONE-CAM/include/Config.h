#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

#define triggerPin 1                        // Pin used to trigger the camera
#define reportPin 3                         // Pin used to report back to main controller
#define statusLEDPin 33                     // Pin used to show the status of the system
#define fileSaveBlinkCount 4                // Number of times the LED blinks after saving a file

// Time intervals for blinking patterns
#define errorOnDuration 500               // 500 ms for "Error" LED on
#define errorOffDuration 50               // 50 ms for "Error" LED off

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
