//include/ADC.h

#ifndef ADC_H
#define ADC_H

#include "config.h"
#include <SPI.h>
#include <Adafruit_ADS1X15.h>

void setupADC();
float readADCChannel(int channel);

#endif // ADC_H