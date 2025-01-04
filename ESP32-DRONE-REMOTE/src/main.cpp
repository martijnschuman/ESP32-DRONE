// main.cpp
#include "config.h"
#include "status.h"
#include "serial.h"
#include "joystick.h"
#include "LCD.h"
#include "buttons.h"
#include "ESPNow.h"

DroneState droneState = {CALIBRATING, BOOT}; 	// Explicit initialization
TelemetryPacket telemetry = {};             	// Zero-initialized
ControlPacket control = {};
bool isConnectedToDrone = false;

void setup() {
	serialSetup();
	setupStatusLEDs();

	setupESPNow();

	// setupLCD();
	setupButtons();
	setupJoysticks();

	Serial.println("Setup complete.");
	setStatus(CALIBRATING);
}

void loop() {
    unsigned long currentTime = millis();

    if (getStatus() == CALIBRATING) {
        if (!leftCalibrated || !rightCalibrated) {
            // calibrationMenu();
			startCalibrateJoysticks();
        }

		if (leftCalibrated && rightCalibrated) {
			setStatus(START_CONNECTION);
			setFlightMode(BOOT);
		}
    }

    if (getStatus() == START_CONNECTION && getFlightMode() == BOOT) {
        static unsigned long lastConnectionAttempt = 0;

        if (currentTime - lastConnectionAttempt >= FIRST_CONNECTION_INTERVAL) {
            lastConnectionAttempt = currentTime;
			Serial.println("Attempting to connect to drone.");

            connectToDrone();
        }

		if (isConnectedToDrone) {
			Serial.println("Connected to drone in loop.");

			setStatus(READY);
			setFlightMode(GROUND);	
		}
    }

    // if (getStatus() == READY && getFlightMode() == GROUND) {
    //     Serial.println("Ready to take off.");
	// 	Serial.println("Press OK to enable manual flight.");
	// 	displayOKStatus();

    //     if (checkOKButton()) {
    //         setFlightMode(MANUAL);
    //     }
    // }

    // if (getStatus() == READY && getFlightMode() == MANUAL) {
    //     static unsigned long lastControlTime = 0;

    //     if (currentTime - lastControlTime >= TRANSMISSION_INTERVAL) {
    //         lastControlTime = currentTime;
    //         sendControl();
    //     }
    // }
}
