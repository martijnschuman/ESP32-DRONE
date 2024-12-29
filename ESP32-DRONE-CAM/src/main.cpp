#include "Config.h"
#include "Cam.h"
#include "Status.h"

bool checkTriggerPin() {
    static unsigned long lastPressTime = 0;
    unsigned long currentTime = millis();

    if (digitalRead(triggerPin) == LOW && (currentTime - lastPressTime) > 50) {
        lastPressTime = currentTime;
        return true;
    }
    return false;
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
