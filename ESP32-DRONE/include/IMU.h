#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "status.h"
#include <I2Cdev.h>
#include <MadgwickAHRS.h>
#include "MPU6050_6Axis_MotionApps20.h"

// Global orientation angles (in degrees) for PID control:
extern float roll, pitch, yaw;
extern float gyroZBias;       // °/s  (set during bias calibration)
extern float currentYawRate;  // °/s  (updated each IMU cycle)

void setupIMU();
void updateIMU();
void calibrateIMUOffsets();

#endif // IMU_H
