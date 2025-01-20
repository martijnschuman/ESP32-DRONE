// include/ESPNow.h

#ifndef ESPNow_H
#define ESPNow_H

#include "config.h"
#include "status.h"
#include <WiFi.h>
#include <esp_now.h>

extern uint8_t remoteMAC[];

void setupESPNow();
void addPeer(uint8_t *peerMAC);
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void sendResponseToRemote();

#endif