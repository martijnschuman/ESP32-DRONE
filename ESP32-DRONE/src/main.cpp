// src/main.cpp

#include <Arduino.h>
#include "mpu6050.h"

// Initialize serial communication
bool serialSetup() {
	Serial.begin(115200);
	unsigned long startTime = millis();
	while (!Serial) {
		if (millis() - startTime > 5000) {
			return false;
		}
		delay(10);
	}
	return true;
}

// Initialize MPU6050
void mpuSetup(){
    if (!initializeMPU6050()) {
        while (1) {
            delay(10);
        }
    }
}

void setup(void) {
    serialSetup();
	mpuSetup();
}

void loop() {
    sensors_event_t a, g, temp;
    readMPU6050(&a, &g, &temp);

    // Print out the values
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");

    Serial.println("");
    delay(500);
}