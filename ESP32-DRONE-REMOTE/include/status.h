// include/status.h
#ifndef STATUS_H
#define STATUS_H

#include "config.h"

enum StatusEnum {
    READY = 0x00,
    I2C_MULTIPLEXER_ERROR = 0x01,
    IMU_ERROR = 0x02,
    LIDAR_ERROR = 0x03,
    GPS_ERROR = 0x04,
    ESP_NOW_INIT_ERROR = 0x05,
    ESP_NOW_ADD_PEER_ERROR = 0x06,
    ESP_NOW_SEND_ERROR = 0x07,
};

void setupStatusLEDs();
void displayOKStatus();
void displayErrorStatus();
void displayBlankStatus();

#endif