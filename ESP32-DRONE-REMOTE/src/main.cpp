#include <Arduino.h>
#include <Serial.h>
#include "Config.h"
#include "Joystick.h"

void setup() {
	serialSetup();

	setupJoysticks();
}

void loop() {
	if(checkStartCalibrationButton() && !getLeftCalibrated() && !getRightCalibrated()) {
		digitalWrite(CALIBRATION_RED_LED, HIGH);
    	digitalWrite(CALIBRATION_GREEN_LED, LOW);

		calibrateLeftJoystick();
		calibrateRightJoystick();

		digitalWrite(CALIBRATION_RED_LED, LOW);
		digitalWrite(CALIBRATION_GREEN_LED, HIGH);

		delay(2000);

		digitalWrite(CALIBRATION_RED_LED, LOW);
		digitalWrite(CALIBRATION_GREEN_LED, LOW);
	}

	if(getLeftCalibrated()) {
		Serial.print("L (X, Y): ( " + String(transferredLeftJoystickReadX()) + ", " + String(transferredLeftJoystickReadY()) + " )");
	}
	if(getRightCalibrated()) {
		Serial.print("R (X, Y): ( " + String(transferredRightJoystickReadX()) + ", " + String(transferredRightJoystickReadY()) + " )");
	}
	if(getLeftCalibrated() && getRightCalibrated()) {
		Serial.println();
	}

	// Serial.print("Right Joystick X: ");
	// Serial.println(rightJoystickReadX());
	// Serial.print("Right Joystick Y: ");
	// Serial.println(rightJoystickReadY());
	delay(1000);
}
