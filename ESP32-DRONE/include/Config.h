// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>

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
#define CAMERA_KEEP_ALIVE_INTERVAL 5000             // Interval to send keep-alive signal to the camera in milliseconds
#define CAMERA_KEEP_ALIVE_TIMEOUT 5000              // Timeout duration to wait for keep alive confirmation in milliseconds


#endif // CONFIG_H