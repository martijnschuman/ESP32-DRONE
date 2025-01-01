// src/sensors/LIDAR.cpp
#include <Arduino.h>
#include <LIDAR.h>

DFRobot_LIDAR07_IIC lidar;

float lidarBuffer[BUFFER_SIZE];         // Circular buffer for distance readings
int lidarIndex = 0;                     // Current index in the buffer
float lidarSum = 0;                     // Sum of the buffer values for averaging

float lidarHeight = 0.0f;

void setupLIDAR(){
    Wire.begin();

    if (!lidar.begin()) {
        Serial.println("LIDAR initialization failed!");
        throwError(LIDAR_ERROR);
        while (1);
    }
    Serial.println("LIDAR initialized successfully.");
}

void updateLIDAR() {
    lidar.startMeasure();

    static int failCount = 0;

    if (lidar.getValue()) { // Get valid distance data
        failCount = 0;

        // Get the latest distance reading
        float newDistance = lidar.getDistanceMM();

        // Update the rolling buffer
        lidarSum -= lidarBuffer[lidarIndex];       // Remove the old value from the sum
        lidarBuffer[lidarIndex] = newDistance;     // Store the new value in the buffer
        lidarSum += lidarBuffer[lidarIndex];       // Add the new value to the sum

        // Move to the next index (circular buffer)
        lidarIndex = (lidarIndex + 1) % BUFFER_SIZE;

        // Calculate and return the rolling average
        lidarHeight = lidarSum / BUFFER_SIZE;
    } else {
        Serial.println("Failed to get distance data. Failed attempts: " + String(failCount));
        failCount++;

        if (failCount >= LIDAR_ALLOWED_FAIL_COUNT) {
            throwError(LIDAR_ERROR);
        }
    }
}
