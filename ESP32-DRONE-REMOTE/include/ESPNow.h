// include/ESPNow.h

#ifndef ESPNow_H
#define ESPNow_H

#include "config.h"
#include <esp_now.h>
#include <WiFi.h>

void setupESPNow();
void addPeer(uint8_t *peerMAC);
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status);
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen);

#endif