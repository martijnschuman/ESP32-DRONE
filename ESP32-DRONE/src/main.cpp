// src/main.cpp

#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <GPS.h>
#include <Serial.h>
#include <Echo.h>


void setup(void)
{
    serialSetup();
    // setupIMU();
    // setupGPS();

    echoSetup();
}

void loop()
{
    // sensors_event_t a, g, temp;
    // readMPU6050(&a, &g, &temp);
    // printMPU6050Data(&a, &g, &temp);

    // displayGPSData();
    // displaySatelliteCount();

    measureDistance();

    delay(500);
}