// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include "global.h"

// Loop intervals in milliseconds
#define BUFFER_SIZE 10                              // Buffer size for IMU and LIDAR data
#define IMU_INTERVAL 100                            // Interval between IMU measurements in milliseconds
#define LIDAR_INTERVAL 250                          // Interval between LIDAR measurements in milliseconds
#define LIDAR_ALLOWED_FAIL_COUNT 5                  // Number of consecutive LIDAR failures allowed before setting status to LIDAR_ERROR
#define TRANSMIT_INTERVAL 500                       // Interval to transmit telemetry data in milliseconds
#define SERIAL_DEBUG_INTERVAL 500                   // Interval to print debug information in milliseconds
#define BATTERY_MONITOR_INTERVAL 1000               // Interval to monitor battery voltage in milliseconds

// Battery
#define BATTERY_VOLTAGE_PIN 15                      // Pin to read battery voltage
#define BATTERY_DIVIDER_RATIO 4.0                   // Voltage divider ratio
#define BATTERY_FULL_VOLTAGE 12.6                   // Full battery voltage
#define BATTERY_LOW_VOLTAGE 10.4                    // Low battery voltage
#define BATTERY_CRITICAL_VOLTAGE 9.9                // Critical battery voltage
#define BATTERY_EMPTY_VOLTAGE 9.0                   // Empty battery voltage
#define BATTERY_SETUP_READINGS 10                   // Number of readings to average for battery voltage during setup
#define BATTERY_SETUP_INTERVAL 10                   // Interval between battery voltage readings during setup in milliseconds
#define BATTERY_MONITOR_READINGS 10                 // Number of readings to average for battery voltage during monitoring

// ESPNow
#define DRONE_MAC_ADDRESS {0xD0, 0xEF, 0x76, 0x34, 0x97, 0x44}
#define REMOTE_MAC_ADDRESS {0xCC, 0xDB, 0xA7, 0x3E, 0x66, 0x84}
#define CONNECTION_TEST_INTERVAL 5000

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