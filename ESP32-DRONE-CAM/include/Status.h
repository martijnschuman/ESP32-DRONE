// include/status.h
#ifndef STATUS_H
#define STATUS_H

#include "config.h"
#include "ESPNow.h"

enum LEDStatusEnum {
    LED_READY,
    LED_SYSTEM_ERROR,
    LED_ESP_NOW_ERROR,
    LED_FILE_SAVED,
};

void setStatus(CommandStatusEnum newStatus);
void reportStatus();
void showLEDStatus(LEDStatusEnum ledStatus);

#endif
