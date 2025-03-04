// main.cpp
#include "config.h"
#include "status.h"
#include "serial.h"
#include "ADC.h"
#include "joystick.h"
#include "display.h"
#include "buttons.h"
#include "ESPNow.h"

DroneState droneState = {CALIBRATING, BOOT};

TelemetryPacket telemetryPacket = {};
ControlPacket controlPacket = {};
DroneStatePacket droneStatePacket = {};
CameraPacket cameraPacket = {};

int takenPictureCount;
long lastPictureTaken;

bool isConnectedToDrone = false;
bool isConnectedToCam = false;

bool isArming = false;

void setup() {
	serialSetup();
	setupStatusLEDs();

	setupESPNow();

    setupADC();

    setupJoysticks();
	setupDisplay();
	setupButtons();

	Serial.println("Setup complete.");
	setStatus(CALIBRATING);
}

// Flow
// 1. Calibrate joysticks
// 2. Connect to drone
// 3. Send flight mode to drone, flight mode is set to MANUAL
// 4. Send control data to drone
void loop() {
    unsigned long currentTime = millis();

    if (isConnectedToDrone && (leftCalibrated && rightCalibrated)) {
        if (isConnectedToCam) {
            if(checkAltButton()) {
                if (currentTime - lastPictureTaken >= CAMERA_PICTURE_INTERVAL) {
                    lastPictureTaken = millis();
                    sendCameraCommand(CAM_TAKE_PICTURE);
                }
            }
        }
        else {
            static unsigned long lastConnectionAttempt = 0;
            if (currentTime - lastConnectionAttempt >= CAMERA_CONNECTION_INTERVAL) {
                // todo this is only shown really quickly
                displayStartCameraConnection();

                Serial.println("Attempting to connect to camera.");

                connectToCamera();
                lastConnectionAttempt = currentTime;
                return;
            }
        }

        // If the remote is ready to fly
        if (getStatus() == READY) {
            // If the remote is in manual mode
            if (getFlightMode() == GROUND) {
                if (checkOKButton()) {
                    sendFlightModeToDrone(GROUND);
                    isArming = true;
                }

                displayLEDOKStatus();

                if (isArming) {
                    displayArmingDrone();
                } else {
                    displayReadyToFly();
                }
            }
            else if (getFlightMode() == MANUAL) {
                static unsigned long lastControlTime = 0;
                isArming = false;

                if (currentTime - lastControlTime >= TRANSMISSION_INTERVAL) {
                    lastControlTime = currentTime;
                    sendControl();
                    displayTelemetry();
                }
            }
        }
    }
    else {
        if (getStatus() == CALIBRATING) {
            displayStartCalibration();

            if ((!leftCalibrated || !rightCalibrated) && (getLeftJoystickButton() && getRightJoystickButton())) {
                startCalibrateJoysticks();
            }

            if (leftCalibrated && rightCalibrated) {
                setStatus(START_CONNECTION);
            }
        }
        else if (getStatus() == START_CONNECTION) {
            static unsigned long lastConnectionAttempt = 0;

            if (currentTime - lastConnectionAttempt >= FIRST_CONNECTION_INTERVAL) {
                displayStartDroneConnection();

                Serial.println("Attempting to connect to drone.");

                connectToDrone();
                lastConnectionAttempt = currentTime;
            }
        }
    }
}
