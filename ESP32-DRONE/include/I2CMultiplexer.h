#ifndef I2CMULTIPLEXER_H
#define I2CMULTIPLEXER_H

#include <Wire.h>
#include <Arduino.h>
#include "TCA9548.h"
#include "Config.h"

void setupI2CMultiplexer();
void enableI2CChannel(uint8_t channel);

#endif // I2CMULTIPLEXER_H