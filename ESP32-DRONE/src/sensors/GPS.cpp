// src/gps.cpp
#include "GPS.h"
#include <Arduino.h>

TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // UART2 (or use Serial1 if preferred)

void setupGPS()
{
    gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // GPS module at 9600 baud, RX2 = GPIO16, TX2 = GPIO17

    if (!gpsSerial)
    {
        Serial.println("Failed to initialize GPS Serial communication!");
        while (true)
            ; // Halt the program
    }

    Serial.println("GPS Module Initialized");
}

void displayGPSData()
{
    // Print out GPS data if available
    if (gps.location.isUpdated())
    {
        Serial.print("Location: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print(", ");
        Serial.print(gps.location.lng(), 6);
        Serial.println();
    }
    if (gps.date.isUpdated())
    {
        Serial.print("Date: ");
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.println(gps.date.year());
    }
    if (gps.time.isUpdated())
    {
        Serial.print("Time: ");
        Serial.print(gps.time.hour());
        Serial.print(":");
        Serial.print(gps.time.minute());
        Serial.print(":");
        Serial.println(gps.time.second());
    }
    if (gps.speed.isUpdated())
    {
        Serial.print("Speed (km/h): ");
        Serial.println(gps.speed.kmph());
    }
}

void displaySatelliteCount()
{
    // Check satellite count
    if (gps.satellites.isUpdated())
    {
        Serial.print("Satellites visible: ");
        Serial.println(gps.satellites.value());
    }
    else
    {
        Serial.println("Waiting for satellite data...");
    }
}