// include/telemetry.h

#ifndef TELEMTRY_H
#define TELEMTRY_H

#include "config.h"
#include "ESPNow.h"
#include "status.h"
#include "joystick.h"

int getThrottleValue();
void sendControl();

#endif // TELEMTRY_H