// ADC.cpp

#include "ADC.h"

Adafruit_ADS1115 ads;

// Initialize the ADC
void setupADC() {
    ads.setGain(ADC_GAIN);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    
    if (!ads.begin(ADC_ADDRESS)) {
        Serial.println("Failed to initialize ADS.");
        while (1);
    }
}

float readADCChannel(int channel) {
    return ads.readADC_SingleEnded(channel) * 0.125;
}
