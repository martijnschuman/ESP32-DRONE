// include/ESPNow.h

#ifndef ESPNow_H
#define ESPNow_H

#include "Config.h"
#include <esp_now.h>
#include <WiFi.h>

extern bool keepAliveAckReceived;

void setupESPNow();
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status);
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void sendKeepAlive();
void sendTakePictureCommand();

#endif