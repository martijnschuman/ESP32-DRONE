// include/ESPNow.h

#ifndef ESPNow_H
#define ESPNow_H

#include "config.h"
#include <esp_now.h>
#include <WiFi.h>
#include "powerMonitor.h"
#include "IMU.h"
#include "echo.h"

extern uint8_t remoteMAC[];

extern unsigned long lastCommunicationTime;

extern float ESCONE_THROTTLE;
extern float ESCTWO_THROTTLE;
extern float ESCTHREE_THROTTLE;
extern float ESCFOUR_THROTTLE;

void setupESPNow();
void addPeer(uint8_t *peerMAC);
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status);
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen);
void sendDroneConnectionReady();
void sendDroneFlightReady();
void sendTelemetry();

#endif