//sensors/echo.cpp

#include "echo.h"

float height = 0;

void setupEcho() {
    pinMode(ECHO_TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_ECHO_PIN, INPUT);
}

void updateHeight() {
    digitalWrite(ECHO_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ECHO_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ECHO_TRIGGER_PIN, LOW);

    float duration = pulseIn(ECHO_ECHO_PIN, HIGH);
    float raw_height = (duration * 0.034 / 2.0);

    if (raw_height < 0) {
        raw_height = 0;
    }
    
    height = raw_height;
}