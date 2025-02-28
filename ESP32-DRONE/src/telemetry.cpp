// telemetry.cpp

#include "telemetry.h"

void printTelemetry() {
    Serial.print("IMU: ");
    Serial.print(accX);
    Serial.print(", ");
    Serial.print(accY);
    Serial.print(", ");
    Serial.print(accZ);
    Serial.print(", ");
    Serial.print(gyroX);
    Serial.print(", ");
    Serial.print(gyroY);
    Serial.print(", ");
    Serial.print(gyroZ);

    Serial.print("LIDAR: ");
    //Serial.println(lidarHeight);
}
