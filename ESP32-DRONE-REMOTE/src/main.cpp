#include <Arduino.h>
#include <Serial.h>
#include "Config.h"
#include "Joystick.h"

void setup() {
	serialSetup();

	setupJoysticks();
}

void loop() {
	if(checkStartCalibrationButton() && !getLeftCalibrated()) {
		calibrateLeftJoystick();
	}

	if(getLeftCalibrated()) {
		Serial.print("Left Joystick X: ");
		Serial.println(transferredLeftJoystickReadX());
		Serial.print("Left Joystick Y: ");
		Serial.println(transferredLeftJoystickReadY());
	}

	// Serial.print("Right Joystick X: ");
	// Serial.println(rightJoystickReadX());
	// Serial.print("Right Joystick Y: ");
	// Serial.println(rightJoystickReadY());
	delay(1000);
}
