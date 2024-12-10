// include/gps.h

#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

extern TinyGPSPlus gps;
extern HardwareSerial gpsSerial;

void setupGPS();
void displayGPSData();
void displaySatelliteCount();

#endif // GPS_H