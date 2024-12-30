// src/main.cpp
#include <Arduino.h>
#include "TinyGPS++.h"
#include "HardwareSerial.h"
#include "GPS.h"
#include "Serial.h"
#include "LIDAR.h"
#include "IMU.h"
#include "I2CMultiplexer.h"
#include "ESPNow.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


void setup(void) {
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

    setupSerial();
    setupI2CMultiplexer();

    enableI2CChannel(MPU6050_CHANNEL);
    enableI2CChannel(LIDAR_CHANNEL);

    setupIMU();
    // setupGPS();
    setupLIDAR();

    // setupESPNow();

    Serial.println("Setup complete.");
}

void loop() {
    sensors_event_t a, g, temp;
    readMPU6050(&a, &g, &temp);
    printMPU6050Data(&a, &g, &temp);

    // displayGPSData();

    measureHight();

    delay(1000);

    // sendIsCamAlive();

    // static bool pictureTriggered = false;

    // if (!pictureTriggered && isAliveAckReceived) {
    //     sendTakePictureCommand();
    //     pictureTriggered = true; // To avoid repeatedly sending the command
    // }
}