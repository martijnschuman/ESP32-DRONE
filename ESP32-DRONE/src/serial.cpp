// src/serial.cpp
#include <Arduino.h>

bool setupSerial()
{
	Serial.begin(115200);
	unsigned long startTime = millis();
	while (!Serial)
	{
		if (millis() - startTime > 5000)
		{
			return false;
		}
		delay(10);
	}
	Serial.println("Serial communication initialized");
	return true;
}
