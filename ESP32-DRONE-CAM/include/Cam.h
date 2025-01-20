// include/cam.h
#ifndef CAM_H
#define CAM_H

#include "config.h"
#include "ESPNow.h"
#include "status.h"
#include "esp_camera.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <EEPROM.h>

void setupCamera();
void setupSDCard();
void takePicture();

#endif
