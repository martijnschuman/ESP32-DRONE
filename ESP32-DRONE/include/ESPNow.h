// include/ESPNow.h

#ifndef ESPNow_H
#define ESPNow_H

#include "config.h"
#include <esp_now.h>
#include <WiFi.h>
#include "telemetry.h"
#include "battery.h"
#include "powerMonitor.h"

extern uint8_t remoteMAC[];

void setupESPNow();
void addPeer(uint8_t *peerMAC);
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status);
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void sendDroneConnectionReady();
void sendDroneFlightReady();
void sendTelemetry();

#endif