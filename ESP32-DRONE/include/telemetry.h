// include/telemetry.h

#ifndef TELEMTRY_H
#define TELEMTRY_H

#include "config.h"
#include "IMU.h"
#include "LIDAR.h"
#include "GPS.h"
#include "ESPNow.h"

// Struct for telemetry packet
struct TelemetryPacket {
    StatusEnum status;
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;
    float temp;
    float height;
    float gpsLat, gpsLng, gpsAlt;
    float gpsSpeed;
    int gpsSatellites;
    String gpsTime, gpsDate;
};

// Struct for control packet
struct ControlPacket {
    float joystickX;
    float joystickY;
    float throttle;
    float yaw;
};

// Define telemetry packet
extern TelemetryPacket telemetry;
extern ControlPacket control;

void printTelemetry();
void transmitTelemetry();

#endif // TELEMTRY_H