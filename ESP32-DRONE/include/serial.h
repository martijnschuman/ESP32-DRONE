// include/Serial.h

#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>
#include "PIDControl.h"

bool setupSerial();
void checkSerialCommands();

#endif // SERIAL_H