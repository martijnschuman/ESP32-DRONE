// include/main.h

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

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

struct CameraPacket {
    CameraMode cameraMode;
};

extern CameraPacket cameraPacket;
extern CameraMode status;

extern unsigned int pictureNumber;

#endif // MAIN_H