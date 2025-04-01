#ifndef IMU_H
#define IMU_H

#include "config.h"
#include <Arduino.h>
#include <I2Cdev.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "status.h"

// Global orientation angles (in degrees) for PID control:
extern float roll, pitch, yaw;

void setupIMU();
void updateIMU();
void calibrateIMUOffsets();

#endif // IMU_H
