//include/PIDControl.h

#ifndef PIDControl_H
#define PIDControl_H

#include "config.h"
#include "IMU.h"
#include <QuickPID.h>
#include "ESC.h"

void updatePIDControl(float dt);

#endif // PIDControl_H