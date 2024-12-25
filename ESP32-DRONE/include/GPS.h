// include/gps.h

#ifndef GPS_H
#define GPS_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

extern TinyGPSPlus gps;
extern HardwareSerial GPSModule;

void setupGPS();
String getGPSTime();
String getGPSDate();
String getGPSLocation();
String getGPSSpeed();
String getGPSAltitude();
String getGPSSatellites();
String getGPSHDOP();
void displayGPSData();

#endif // GPS_H