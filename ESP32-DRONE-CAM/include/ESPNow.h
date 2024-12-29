// include/ESPNow.h

#ifndef ESPNow_H
#define ESPNow_H

#include "config.h"
#include "status.h"
#include <WiFi.h>
#include <esp_now.h>

extern bool cameraConnected;
extern uint8_t mainMAC[];

void setupESPNow();
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void sendStatusToMaster();

#endif