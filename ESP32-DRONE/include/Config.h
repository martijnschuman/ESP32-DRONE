// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include "global.h"

// Loop intervals in milliseconds
#define BUFFER_SIZE 10
#define IMU_INTERVAL 100
#define LIDAR_INTERVAL 250
#define LIDAR_ALLOWED_FAIL_COUNT 5
#define TRANSMIT_INTERVAL 100
#define SERIAL_DEBUG_INTERVAL 500
#define CONNECTION_CHECK_INTERVAL 1000

// ESPNow
#define DRONE_MAC_ADDRESS {0xD0, 0xEF, 0x76, 0x34, 0x97, 0x44}
#define REMOTE_MAC_ADDRESS {0xCC, 0xDB, 0xA7, 0x3E, 0x66, 0x84}
#define CONNECTION_TEST_INTERVAL 5000

// I2C Multiplexer
#define TCA9548A_ADDRESS 0x75                       // I2C address of the multiplexer
#define MPU6050_CHANNEL 0                           // Multiplexer channel for the MPU6050
#define LIDAR_CHANNEL 1                             // Multiplexer channel for the LIDAR

// LIDAR
#define LIDAR_MEASUREMENT_INTERVAL 250              // Interval between LIDAR measurements in milliseconds

// GPS
#define GPS_RXD2 16                                 // GPS module RX pin
#define GPS_TXD2 17                                 // GPS module TX pin

// Camera
#define CAMERA_TRIGGER_PIN 18                       // Pin to send the trigger signal to the slave
#define CAMERA_RECEIVE_STATUS_PIN 5                 // Pin to receive the status signal from the slave
#define CAMERA_TIMEOUT 5000                         // Timeout duration to wait for confirmation in milliseconds
#define CAMERA_CONFIRMATION_PULSE_COUNT 4           // Number of pulses to detect for confirmation
#define CAMERA_KEEP_ALIVE_INTERVAL 10000            // Interval to send keep-alive signal to the camera in milliseconds
#define CAMERA_KEEP_ALIVE_TIMEOUT 5000              // Timeout duration to wait for keep alive confirmation in milliseconds

// Status Display
#define SHIFT_REGISTER_DS_PIN 14                    // Serial data out pin for the shift register
#define SHIFT_REGISTER_SHCP_PIN 13                  // Shift register clock - SRCLK pin
#define SHIFT_REGISTER_STCP_PIN 12                  // Storage register clock - RCLK pin
#define STATUS_REPORT_INTERVAL 1000                 // Interval to report status in milliseconds

#endif // CONFIG_H