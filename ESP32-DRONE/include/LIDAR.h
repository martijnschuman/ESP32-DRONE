// include/LIDAR.h

#ifndef LIDAR_H
#define LIDAR_H

#include "config.h"
#include <DFRobot_LIDAR07.h>
#include "I2CMultiplexer.h"

extern DFRobot_LIDAR07_IIC lidar;

extern float lidarHeight;

void setupLIDAR();
void updateLIDAR();

#endif // LIDAR_H