// src/main.cpp

#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <GPS.h>
#include <Serial.h>


void setup(void)
{
    serialSetup();
    // setupIMU();

    setupGPS();
}

void loop()
{
    // sensors_event_t a, g, temp;
    // readMPU6050(&a, &g, &temp);
    // printMPU6050Data(&a, &g, &temp);

    // displayGPSData();
    // displaySatelliteCount();

    delay(500);
}