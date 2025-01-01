// include/IMU.h

#ifndef MPU6050_H
#define MPU6050_H

#include "config.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "I2CMultiplexer.h"
#include "status.h"

extern Adafruit_MPU6050 mpu;

extern float accX, accY, accZ;
extern float gyroX, gyroY, gyroZ;
extern float temp;

void setupIMU();
bool initializeMPU6050();
void updateIMU();

#endif // MPU6050_H