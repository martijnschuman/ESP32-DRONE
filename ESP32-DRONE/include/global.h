// include/main.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

enum StatusEnum {
    CALIBRATING = 0x00,
    START_CONNECTION = 0x01,
    READY = 0x02,
    IMU_ERROR = 0x03,
    ECHO_ERROR = 0x04,
    ESP_NOW_INIT_ERROR = 0x05,
    ESP_COMMS_ERROR = 0x06,
    ADC_ERROR = 0x07,
    DISPLAY_ERROR = 0x08,
    EMERGENCY = 0x09,
};

enum FlightMode {
    BOOT = 0x00,
    GROUND = 0x01,
    MANUAL = 0x02,
    EMERGENCY_DECENT = 0x03,
};

struct DroneState {
    StatusEnum status;
    FlightMode flightMode;
};

// Struct for telemetry packet
struct TelemetryPacket {
    StatusEnum status;
    FlightMode flightMode;
    float roll, pitch, yaw;
    float height;
    float current;
    float busVoltage;
    float shuntVoltage;
    float ESCOneThrottle, ESCTwoThrottle, ESCThreeThrottle, ESCFourThrottle;
};

// Struct for control packet
struct ControlPacket {
    float throttle;
    float yaw;
    float roll;
    float pitch;
};

struct DroneStatePacket {
    DroneState droneState;
};

extern DroneState droneState;

extern ControlPacket controlPacket;
extern DroneStatePacket droneStatePacket;

extern bool isConnectedToRemote;

#endif // MAIN_H