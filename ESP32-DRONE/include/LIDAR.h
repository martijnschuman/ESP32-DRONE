// include/LIDAR.h

#ifndef LIDAR_H
#define LIDAR_H

#include "Config.h"
#include <DFRobot_LIDAR07.h>
#include "I2CMultiplexer.h"

extern DFRobot_LIDAR07_IIC lidar;

void setupLIDAR();
void measureHight();

#endif // LIDAR_H