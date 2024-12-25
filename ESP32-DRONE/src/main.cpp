// src/main.cpp

#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <GPS.h>
#include <Serial.h>
#include <LIDAR.h>

void setup(void)
{
    serialSetup();
    // setupIMU();
    // setupGPS();
    // echoSetup();
    // setupLIDAR();

    // Serial.println("Setup done");
}

void loop()
{
    // sensors_event_t a, g, temp;
    // readMPU6050(&a, &g, &temp);
    // printMPU6050Data(&a, &g, &temp);

    // displayGPSData();

    // measureHight();

    delay(500);
}