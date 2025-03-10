//include/PIDControl.h

#ifndef PIDControl_H
#define PIDControl_H

#include "config.h"
#include "IMU.h"
#include <QuickPID.h>
#include "ESC.h"

void setupPIDControl();
void updatePIDControl();

#endif // PIDControl_H