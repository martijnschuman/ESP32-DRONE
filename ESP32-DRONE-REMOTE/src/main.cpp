// main.cpp
#include "config.h"
#include "status.h"
#include "serial.h"
#include "I2CMultiplexer.h"
#include "ADC.h"
#include "joystick.h"
#include "display.h"
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

    setupI2CMultiplexer();
    enableI2CChannel(ADC_CHANNEL);
    enableI2CChannel(DISPLAY_CHANNEL);

    setupADC();

    setupJoysticks();
	setupDisplay();
	setupButtons();

	Serial.println("Setup complete.");
	setStatus(CALIBRATING);
}

void loop() {
    Serial.println(getLeftJoystickButton());
    Serial.println(getRightJoystickButton());
    delay(1000);
}

// void loop() {
//     display.clearDisplay();
//     display.display();

//     display.setCursor(0, 0);
//     display.println("Hello, world!");

//     display.display();
//     delay(2000);
// }

// void loop() {
//     unsigned long currentTime = millis();

//     if (getStatus() == CALIBRATING) {
//         if (!leftCalibrated || !rightCalibrated) {
// 			startCalibrateJoysticks();
//         }

// 		if (leftCalibrated && rightCalibrated) {
// 			setStatus(START_CONNECTION);
// 			setFlightMode(BOOT);
// 		}
//     }

//     if (getStatus() == START_CONNECTION && getFlightMode() == BOOT) {
//         static unsigned long lastConnectionAttempt = 0;

//         if (currentTime - lastConnectionAttempt >= FIRST_CONNECTION_INTERVAL) {
//             lastConnectionAttempt = currentTime;
// 			Serial.println("Attempting to connect to drone.");

//             connectToDrone();
//         }
//     }

//     if (isConnectedToDrone) {
//         if (getStatus() == READY && getFlightMode() == GROUND) {
//             Serial.println("Press OK to enable manual flight.");
//             displayOKStatus();

//             if (checkOKButton()) {
//                 setFlightMode(MANUAL);
//                 sendFlightModeToDrone(MANUAL);
//             }
//         }

//         if (getStatus() == READY && getFlightMode() == MANUAL) {
//             static unsigned long lastControlTime = 0;

//             if (currentTime - lastControlTime >= TRANSMISSION_INTERVAL) {
//                 lastControlTime = currentTime;
//                 sendControl();
//             }
//         }
//     }
// }
