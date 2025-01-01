// include/I2CMultiplexer.h

#ifndef I2CMULTIPLEXER_H
#define I2CMULTIPLEXER_H

#include "config.h"
#include <TCA9548.h>
#include "status.h"

void setupI2CMultiplexer();
void enableI2CChannel(uint8_t channel);

#endif