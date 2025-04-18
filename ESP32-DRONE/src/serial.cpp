// src/serial.cpp
#include "serial.h"

bool setupSerial() {
	Serial.begin(115200);
	unsigned long startTime = millis();
	while (!Serial) {
		if (millis() - startTime > 5000) {
			return false;
		}
		delay(10);
	}
	Serial.println("Serial communication initialized");
	return true;
}

void checkSerialCommands() {
    static String cmd;
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            cmd.trim();
            processPIDCommand(cmd);
            cmd = "";
        } else {
            cmd += c;
        }
    }
}