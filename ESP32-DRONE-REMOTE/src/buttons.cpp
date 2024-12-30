#include "buttons.h"

void setupButtons() {
    pinMode(BUTTON_OK, INPUT);
    pinMode(BUTTON_ALT, INPUT);
}

bool checkOKButton() {
    static unsigned long lastPressTime = 0;
    unsigned long currentTime = millis();

    if (digitalRead(BUTTON_OK) == HIGH && (currentTime - lastPressTime) > DEBOUNCE_DELAY) {
        lastPressTime = currentTime;
        return true;
    }
    return false;
}

bool checkAltButton() {
    static unsigned long lastPressTime = 0;
    unsigned long currentTime = millis();

    if (digitalRead(BUTTON_ALT) == HIGH && (currentTime - lastPressTime) > DEBOUNCE_DELAY) {
        lastPressTime = currentTime;
        return true;
    }
    return false;
}
