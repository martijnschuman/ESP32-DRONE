// include/gps.h

#ifndef GPS_H
#define GPS_H

#include "config.h"
#include <TinyGPS++.h>
#include <HardwareSerial.h>

extern TinyGPSPlus gps;
extern HardwareSerial GPSModule;

extern float gpsLat, gpsLng, gpsAlt;
extern float gpsSpeed;
extern int gpsSatellites;
extern String gpsTime, gpsDate;

void setupGPS();
void displayGPSData();

#endif // GPS_H