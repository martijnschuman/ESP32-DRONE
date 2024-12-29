// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>

// ESPNow
#define MAIN_MAC_ADDRESS {0xD0, 0xEF, 0x76, 0x34, 0x97, 0x44}
#define CAMERA_MAC_ADDRESS {0xA0, 0xA3, 0xB3, 0xEC, 0xEC, 0x54}

enum CommandType {
    KEEP_ALIVE = 0x01,
    TAKE_PICTURE = 0x02,
};

struct CommandPacket {
    CommandType command;
};

enum StatusEnum {
    READY,
    FILE_SAVED,
    SD_INIT_ERROR,
    SD_SAVE_ERROR,
    CAMERA_INIT_ERROR,
    CAMERA_TAKE_ERROR,
    ESP_NOW_INIT_ERROR,
    ESP_NOW_ADD_PEER_ERROR,
};

// I2C Multiplexer
#define TCA9548A_ADDRESS 0x75                       // I2C address of the multiplexer
#define MPU6050_CHANNEL 0                           // Multiplexer channel for the MPU6050
#define LIDAR_CHANNEL 1                             // Multiplexer channel for the LIDAR

// LIDAR
#define LIDAR_MEASUREMENT_INTERVAL 500              // Interval between LIDAR measurements in milliseconds

// GPS
#define GPS_RXD2 16                                 // GPS module RX pin
#define GPS_TXD2 17                                 // GPS module TX pin

// IMU
#define IMU_TEMP_CALIBRATION -2.5                   // Temperature calibration value for the IMU in degrees Celsius

// Camera
#define CAMERA_TRIGGER_PIN 18                       // Pin to send the trigger signal to the slave
#define CAMERA_RECEIVE_STATUS_PIN 5                 // Pin to receive the status signal from the slave
#define CAMERA_TIMEOUT 5000                         // Timeout duration to wait for confirmation in milliseconds
#define CAMERA_CONFIRMATION_PULSE_COUNT 4           // Number of pulses to detect for confirmation
#define CAMERA_KEEP_ALIVE_INTERVAL 10000             // Interval to send keep-alive signal to the camera in milliseconds
#define CAMERA_KEEP_ALIVE_TIMEOUT 5000              // Timeout duration to wait for keep alive confirmation in milliseconds


#endif // CONFIG_H