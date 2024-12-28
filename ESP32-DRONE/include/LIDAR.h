// include/LIDAR.h

#ifndef LIDAR_H
#define LIDAR_H

#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_LIDAR07.h>
#include <Config.h>
#include <Wire.h>
#include "I2CMultiplexer.h"
#include "Config.h"

extern DFRobot_LIDAR07_IIC lidar;

void setupLIDAR();
void measureHight();

#endif // LIDAR_H