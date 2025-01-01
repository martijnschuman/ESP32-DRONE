// main.cpp
#include "config.h"
#include "serial.h"
#include "joystick.h"
#include "LCD.h"
#include "buttons.h"
#include "ESPNow.h"

bool isDroneConnected = false;

typedef struct message {
	int leftX;
	int leftY;
	int rightX;
	int rightY;
} message;

message handControllerData;

void setup() {
	serialSetup();

	setupLCD();
	setupButtons();
	setupJoysticks();

	Serial.println("Setup complete.");
}

void loop() {
	// if (!leftCalibrated || !rightCalibrated) {
	// 	calibrationMenu();
	// }

	// if (!isDroneConnected) {
	// 	connectToDroneDisplay();

	// 	if (checkOKButton()) {
	// 		isDroneConnected = true;
	// 		connectedToDroneDisplay();
	// 	}
	// } else {
	// 	joystickDebugDisplay();
	// }
	
	// sendIsCamAlive();

    // static bool pictureTriggered = false;

    // if (!pictureTriggered && isAliveAckReceived) {
    //     sendTakePictureCommand();
    //     pictureTriggered = true; // To avoid repeatedly sending the command
    // }
}
