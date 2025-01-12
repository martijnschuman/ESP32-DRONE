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

DroneState droneState = {CALIBRATING, BOOT};

TelemetryPacket telemetryPacket = {};
ControlPacket controlPacket = {};
DroneStatePacket droneStatePacket = {};

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
    unsigned long currentTime = millis();

    if (isConnectedToDrone) {
        if (getStatus() == READY) {
            if (getFlightMode() == GROUND) {
                if (checkOKButton()) {
                    sendFlightModeToDrone(MANUAL);
                }

                displayLEDOKStatus();
                displayReadyToFly();
            }
            else if (getFlightMode() == MANUAL) {
                displayLEDErrorStatus();
                static unsigned long lastControlTime = 0;

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
