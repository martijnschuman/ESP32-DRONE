// include/main.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

enum StatusEnum {
    CALIBRATING = 0x00,
    START_CONNECTION = 0x01,
    READY = 0x02,
    I2C_MULTIPLEXER_ERROR = 0x03,
    IMU_ERROR = 0x04,
    LIDAR_ERROR = 0x05,
    GPS_ERROR = 0x06,
    ESP_NOW_INIT_ERROR = 0x07,
    ESP_NOW_SEND_ERROR = 0x08,
    BATTERY_LOW = 0x09,
    BATTERY_CRITICAL = 0x0A,
};

enum FlightMode {
    BOOT = 0x00,
    GROUND = 0x01,
    MANUAL = 0x02,
    STABILIZE = 0x03,
    ALT_HOLD = 0x04,
    LOITER = 0x05,
    AUTO = 0x06,
    RTL = 0x07,
    LANDING = 0x08,
};

struct DroneState {
    StatusEnum status;
    FlightMode flightMode;
};

// Struct for telemetry packet
struct TelemetryPacket {
    StatusEnum status;
    FlightMode flightMode;
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

struct FirstConnectionRequestPacket {
    StatusEnum status;
};

extern DroneState droneState;

extern TelemetryPacket telemetry;
extern ControlPacket control;
extern FirstConnectionRequestPacket firstConnection;

#endif // MAIN_H