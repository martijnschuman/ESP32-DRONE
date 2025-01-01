//src/I2CMultiplexer
#include "I2CMultiplexer.h"

TCA9548 MP(TCA9548A_ADDRESS);

void scanI2CDevices(uint8_t channel) {
    MP.enableChannel(channel); // Enable the selected channel
    delay(100); // Give time for devices to respond

    Serial.print("Scanning channel ");
    Serial.println(channel);

    bool deviceFound = false;
    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print("  Found I2C device at address 0x");
            Serial.println(address, HEX);
            deviceFound = true;
        }
    }

    if (!deviceFound) {
        Serial.println("  No devices found on this channel.");
    }
    MP.disableChannel(channel); // Disable the channel after scanning
    delay(100);
}

void setupI2CMultiplexer() {
    Wire.begin();
    if (!MP.begin()) {
        Serial.println("Multiplexer not connected!");
        throwError(I2C_MULTIPLEXER_ERROR);
        while (1);
    }
    Serial.println("Multiplexer connected!");

    for (uint8_t channel = 0; channel < 8; channel++) {
        scanI2CDevices(channel);
    }
}

void enableI2CChannel(uint8_t channel) {
    Serial.println("Enabling channel " + String(channel) + "...");
    MP.enableChannel(channel);
}