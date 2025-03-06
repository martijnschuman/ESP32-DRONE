// src/serial.cpp
#include "config.h"

void serialSetup() {
	Serial.begin(115200);

	Serial.println("Serial communication initialized");
}
