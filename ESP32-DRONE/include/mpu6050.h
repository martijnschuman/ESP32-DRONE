// include/mpu6050.h

#ifndef MPU6050_H
#define MPU6050_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

extern Adafruit_MPU6050 mpu;

bool initializeMPU6050();
void readMPU6050(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp);

#endif // MPU6050_H