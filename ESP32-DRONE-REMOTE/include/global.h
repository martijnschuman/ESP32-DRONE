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
    MAG_ERROR = 0x0A
};

enum FlightMode {
    BOOT = 0x00,
    GROUND = 0x01,
    MANUAL = 0x02,
    EMERGENCY_DECENT = 0x03,
};

enum CameraMode {
    CAM_BOOT = 0x00,
    CAM_READY = 0x01,
    CAM_TAKE_PICTURE = 0x02,
    CAM_PICTURE_SAVED = 0x03,
    CAM_ERROR = 0x04,
    CAM_INIT_ERROR = 0x05,
    SD_INIT_ERROR = 0x06,
    SD_SAVE_ERROR = 0x07,
    CAMERA_TAKE_ERROR = 0x08,
    CAM_ESP_NOW_INIT_ERROR = 0x09,
    CAM_ESP_NOW_SEND_ERROR = 0x0A,
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

struct CameraPacket {
    CameraMode cameraMode;
};

extern DroneState droneState;

extern TelemetryPacket telemetryPacket;
extern ControlPacket controlPacket;
extern DroneStatePacket droneStatePacket;
extern CameraPacket cameraPacket;

extern bool isConnectedToDrone;
extern bool isConnectedToCam;

extern int takenPictureCount;
extern long lastPictureTaken;

void hold();
void handleSetupError(StatusEnum errorStatus, const char* errorMessage);

#endif // MAIN_H