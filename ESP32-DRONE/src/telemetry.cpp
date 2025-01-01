// telemetry.cpp

#include "telemetry.h"

// Define telemetry packet
TelemetryPacket telemetry;
ControlPacket control;

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
    Serial.print(", ");
    Serial.println(temp);

    Serial.print("LIDAR: ");
    Serial.println(lidarHeight);
}

void transmitTelemetry() {
    telemetry.status = status;
    telemetry.accX = accX;
    telemetry.accY = accY;
    telemetry.accZ = accZ;
    telemetry.gyroX = gyroX;
    telemetry.gyroY = gyroY;
    telemetry.gyroZ = gyroZ;
    telemetry.temp = temp;
    telemetry.height = lidarHeight;
    telemetry.gpsLat = gpsLat;
    telemetry.gpsLng = gpsLng;
    telemetry.gpsAlt = gpsAlt;
    telemetry.gpsSpeed = gpsSpeed;
    telemetry.gpsSatellites = gpsSatellites;
    telemetry.gpsTime = gpsTime;
    telemetry.gpsDate = gpsDate;

    esp_err_t result = esp_now_send(remoteMAC, reinterpret_cast<uint8_t*>(&telemetry), sizeof(telemetry));
    if (result == ESP_OK) {
        Serial.println("Telemetry sent.");
    } else {
        Serial.println("Error sending telemetry.");
        throwError(ESP_NOW_SEND_ERROR);
    }
}
