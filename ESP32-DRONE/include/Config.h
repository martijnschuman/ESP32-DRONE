// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include "global.h"

// Loop intervals in milliseconds
#define BUFFER_SIZE 5                               // Buffer size for IMU and LIDAR data
#define IMU_INTERVAL 100                            // Interval between IMU measurements in milliseconds
#define ECHO_INTERVAL 200                           // Echo between LIDAR measurements in milliseconds
#define TRANSMIT_INTERVAL 500                       // Interval to transmit telemetry data in milliseconds
#define BATTERY_MONITOR_INTERVAL 1000               // Interval to monitor battery voltage in milliseconds
#define POWER_MONITOR_INTERVAL 750                  // Interval to monitor power in milliseconds

// IMU
#define IMU_SETUP_CALIBRATION_COUNT 500             // Number of samples to calibrate the IMU

// ESPNow
#define DRONE_MAC_ADDRESS {0xD0, 0xEF, 0x76, 0x34, 0x97, 0x44}
#define REMOTE_MAC_ADDRESS {0xCC, 0xDB, 0xA7, 0x3E, 0x66, 0x84}
#define CONNECTION_TEST_INTERVAL 5000

// Echo
#define ECHO_MEASUREMENT_INTERVAL 250              // Interval between echo measurements in milliseconds
#define ECHO_SETUP_INTERVAL 1000                    // Interval between echo setup readings in milliseconds
#define ECHO_SETUP_READINGS 5                      // Number of consecutive echo failures allowed before setting status to echo
#define ECHO_TRIGGER_PIN 19                        // Pin to send the trigger signal to the echo sensor
#define ECHO_ECHO_PIN 18                           // Pin to receive the echo signal from the echo sensor

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

// ESC
#define ESC_ONE_PIN 32                              // Pin for ESC 1
#define ESC_TWO_PIN 33                              // Pin for ESC 2
#define ESC_THREE_PIN 25                            // Pin for ESC 3
#define ESC_FOUR_PIN 27                             // Pin for ESC 4

#define ESC_MIN_THROTTLE 1000                       // Minimum throttle value
#define ESC_MAX_THROTTLE 2000                       // Maximum throttle value
#define ESC_ALLOWED_MAX_THROTTLE 1500               // Maximum throttle value
#define ESC_FREQUENCY 100                           // ESC frequency in Hz

#endif // CONFIG_H