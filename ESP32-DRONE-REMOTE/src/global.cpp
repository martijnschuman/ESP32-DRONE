//global.cpp

#include "global.h"
#include "status.h"

void hold() {
    while (true) {
        delay(100);
    }
}

void handleSetupError(StatusEnum errorStatus, const char* errorMessage) {
    Serial.println(errorMessage);
    setStatus(errorStatus);
    displayLEDErrorStatus();
    hold();
}
