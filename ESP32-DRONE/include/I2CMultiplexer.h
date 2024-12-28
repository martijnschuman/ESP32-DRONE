// include/I2CMultiplexer.h

#ifndef I2CMULTIPLEXER_H
#define I2CMULTIPLEXER_H

#include "Config.h"
#include "TCA9548.h"

void setupI2CMultiplexer();
void enableI2CChannel(uint8_t channel);

#endif // I2CMULTIPLEXER_H