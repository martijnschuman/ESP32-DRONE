// include/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include "global.h"

#define STICK_SENSITIVITY 0.03f
#define MAX_YAW_RATE_DPS  200.0f   // full-stick → ±200 °/s

// Loop intervals
#define IMU_INTERVAL 100                            // Interval between IMU measurements in milliseconds
#define ECHO_INTERVAL 150                           // Echo between LIDAR measurements in milliseconds
#define TRANSMIT_INTERVAL 100                       // Interval to transmit telemetry data in milliseconds
#define POWER_MONITOR_INTERVAL 500                  // Interval to monitor power in milliseconds
#define PID_UPDATE_INTERVAL 20                      // Interval to update PID control in milliseconds
#define STATUS_REPORT_INTERVAL 1000                 // Interval to report status in milliseconds

// Battery Monitor
#define BATTERY_MIN_VOLTAGE 13.25                   // Minimum battery voltage in volts

// ESPNow
#define DRONE_MAC_ADDRESS {0xCC, 0xDB, 0xA7, 0x3E, 0x2D, 0x78}
#define REMOTE_MAC_ADDRESS {0xCC, 0xDB, 0xA7, 0x3E, 0x66, 0x84}
#define CONNECTION_TIMEOUT 1500                     // Timeout for connection in milliseconds -> no message within this time, switch to emergency descent mode

// Echo
#define ECHO_TRIGGER_PIN 19                        // Pin to send the trigger signal to the echo sensor
#define ECHO_ECHO_PIN 18                           // Pin to receive the echo signal from the echo sensor

// Status Display
#define SHIFT_REGISTER_DS_PIN 14                    // Serial data out pin for the shift register
#define SHIFT_REGISTER_SHCP_PIN 13                  // Shift register clock - SRCLK pin
#define SHIFT_REGISTER_STCP_PIN 12                  // Storage register clock - RCLK pin

// ESC
#define ESC_ONE_PIN 27                              // Pin for ESC 1
#define ESC_TWO_PIN 25                              // Pin for ESC 2
#define ESC_THREE_PIN 33                            // Pin for ESC 3
#define ESC_FOUR_PIN 32                             // Pin for ESC 4

#define MIN_MOTOR_OUTPUT_PERCENTAGE 8               // Minimum throttle value during flight in percentage
#define ESC_MIN_THROTTLE_MS 1000                    // Minimum throttle value in microseconds
#define ESC_MAX_THROTTLE_MS 2000                    // Maximum throttle value in microseconds
#define ESC_ALLOWED_MAX_THROTTLE_MS 1450            // Maximum throttle value in microseconds
#define ESC_FREQUENCY 50                            // ESC frequency in Hz

#endif // CONFIG_H
