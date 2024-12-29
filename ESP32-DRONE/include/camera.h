// include/camera.h

#ifndef CAMERA_H
#define CAMERA_H

#include "Config.h"

void setupCamera();
void sendTrigger();
bool waitForConfirmation();
void isCameraAlive();

#endif