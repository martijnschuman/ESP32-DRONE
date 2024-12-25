#include <Arduino.h>
#include <LIDAR.h>

DFRobot_LIDAR07_IIC lidar;

void setupLIDAR(){
    Wire.begin(); // Initialize I2C

    if (!lidar.begin()) {
        Serial.println("LIDAR initialization failed!");
        while (1); // Halt if initialization fails
    }
    Serial.println("LIDAR initialized successfully.");
}

void measureHight(){
    static unsigned long lastDisplayTime = 0;
    static uint16_t distance = 0;
    static int measurementsCount = 0;

    lidar.startMeasure(); // Start measurement

    if (lidar.getValue()) { // Get distance data
        distance += lidar.getDistanceMM(); // Get distance in mm
        measurementsCount++;
    } else {
        Serial.println("Failed to get distance data.");
    }

    unsigned long currentTime = millis();
    if (currentTime - lastDisplayTime >= LIDARMeasurementInterval) {
        Serial.print("Distance: ");
        Serial.print(distance / measurementsCount);
        Serial.println(" mm");
        lastDisplayTime = currentTime;
        distance = 0;
        measurementsCount = 0;
    }
}
