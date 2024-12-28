// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>

// I2C Multiplexer
#define TCA9548A_ADDRESS 0x75
#define MPU6050_CHANNEL 0
#define LIDAR_CHANNEL 1

// LIDAR
#define LIDARMeasurementInterval 500

// GPS
#define GPS_RXD2 16
#define GPS_TXD2 17

// IMU
#define IMU_TEMP_CALIBRATION -2.5

#endif // CONFIG_H