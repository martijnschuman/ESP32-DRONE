// main.cpp
#include "Config.h"
#include "Cam.h"
#include "Status.h"


bool checkTriggerPin() {
    static unsigned long lastPressTime = 0;
    static int triggerStep = 0; // Tracks the current step in the pattern (0: low, 1: high, 2: low, 3: high)
    unsigned long currentTime = millis();

    // Debounce the signal by checking the time since the last change
    if ((currentTime - lastPressTime) > 50) {
        int pinState = digitalRead(triggerPin);

        switch (triggerStep) {
            case 0: // Expecting LOW
                if (pinState == LOW) {
                    triggerStep = 1;
                    lastPressTime = currentTime;
                }
                break;

            case 1: // Expecting HIGH
                if (pinState == HIGH) {
                    triggerStep = 2;
                    lastPressTime = currentTime;
                }
                break;

            case 2: // Expecting LOW
                if (pinState == LOW) {
                    triggerStep = 3;
                    lastPressTime = currentTime;
                }
                break;

            case 3: // Expecting HIGH
                if (pinState == HIGH) {
                    // Pattern detected successfully
                    triggerStep = 0; // Reset for the next detection
                    lastPressTime = currentTime;
                    return true;
                }
                break;

            default:
                triggerStep = 0; // Reset on unexpected behavior
                break;
        }
    }

    return false;
}

void checkIsAlivePulse() {
    static unsigned long lastPulseTime = 0;
    unsigned long currentMillis = millis();

    // Check for a low-to-high transition on the alivePin
    if (digitalRead(triggerPin) == LOW) {
        lastPulseTime = currentMillis; // Reset pulse timer
    } else if (digitalRead(triggerPin) == HIGH && currentMillis - lastPulseTime < 50) {
        // Respond to the master if the pulse is valid
        digitalWrite(reportPin, HIGH);
        delay(10); // Brief high pulse
        digitalWrite(reportPin, LOW);
    }
}

void setup() {
	setupCamera();
	setupSDCard();

	pinMode(triggerPin, INPUT_PULLUP);

	pinMode(statusLEDPin, OUTPUT);
	pinMode(reportPin, OUTPUT);

	if(!validateSDCard()){
		status = SYSTEM_ERROR;
		return;
	}
}

void loop() {
	if (checkTriggerPin()) {
		takePicture();
	}

	reportStatus(status);
}
