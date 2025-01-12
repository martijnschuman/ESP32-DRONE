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
    ADC_ERROR = 0x08,
    DISPLAY_ERROR = 0x09,
    BATTERY_LOW = 0x0A,
    BATTERY_CRITICAL = 0x0B,
};

enum FlightMode {
    BOOT = 0x00,
    GROUND = 0x01,
    MANUAL = 0x02,
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
    float height;
    float gpsLat, gpsLng, gpsAlt;
    float gpsSpeed;
    int gpsSatellites;
    String gpsTime, gpsDate;
    float batteryVoltage;
};

// Struct for control packet
struct ControlPacket {
    float joystickX;
    float joystickY;
    float throttle;
    float yaw;
};

struct DroneStatePacket {
    DroneState droneState;
};

extern DroneState droneState;

extern TelemetryPacket telemetryPacket;
extern ControlPacket controlPacket;
extern DroneStatePacket droneStatePacket;

extern bool isConnectedToDrone;

void hold();
void handleSetupError(StatusEnum errorStatus, const char* errorMessage);

#endif // MAIN_H