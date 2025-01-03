// include/telemetry.h

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "config.h"
#include "ESPNow.h"
#include "status.h"
#include "IMU.h"
#include "LIDAR.h"
#include "GPS.h"


void printTelemetry();
void transmitTelemetry();

#endif // telemetry
