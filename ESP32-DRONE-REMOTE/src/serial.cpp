// src/serial.cpp
#include "config.h"

void serialSetup() {
	Serial.begin(115200);

	Serial.println("Serial communication initialized");
}

void printTelemetry(){
    Serial.print("Status: "); Serial.println(telemetryPacket.status);
    Serial.print("Flight Mode: "); Serial.println(telemetryPacket.flightMode);
    Serial.print("Acc X: "); Serial.println(telemetryPacket.accX);
    Serial.print("Acc Y: "); Serial.println(telemetryPacket.accY);
    Serial.print("Acc Z: "); Serial.println(telemetryPacket.accZ);
    Serial.print("Gyro X: "); Serial.println(telemetryPacket.gyroX);
    Serial.print("Gyro Y: "); Serial.println(telemetryPacket.gyroY);
    Serial.print("Gyro Z: "); Serial.println(telemetryPacket.gyroZ);
    Serial.print("Height: "); Serial.println(telemetryPacket.height);
    Serial.print("GPS Lat: "); Serial.println(telemetryPacket.gpsLat);
    Serial.print("GPS Lng: "); Serial.println(telemetryPacket.gpsLng);
    Serial.print("GPS Alt: "); Serial.println(telemetryPacket.gpsAlt);
    Serial.print("GPS Speed: "); Serial.println(telemetryPacket.gpsSpeed);
    Serial.print("GPS Satellites: "); Serial.println(telemetryPacket.gpsSatellites);
    Serial.print("GPS Time: "); Serial.println(telemetryPacket.gpsTime);
    Serial.print("GPS Date: "); Serial.println(telemetryPacket.gpsDate);
    Serial.print("Battery Voltage: "); Serial.println(telemetryPacket.batteryVoltage);
}
