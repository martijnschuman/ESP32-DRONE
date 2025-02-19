//ESC.cpp

#include "ESC.h"

Servo ESCOne;
Servo ESCTwo;
Servo ESCThree;
Servo ESCFour;


void setupESC(Servo ESC, int ESC_PIN) {
    ESC.setPeriodHertz(ESC_FREQUENCY);
    ESC.attach(ESC_PIN, ESC_MIN_THROTTLE, ESC_MAX_THROTTLE);
}

void armESC(Servo ESC) {
    // Initial throttle setting (this will arm the ESC if it's idle)
    Serial.println("Sending minimum throttle for arming sequence...");
    ESC.writeMicroseconds(ESC_MIN_THROTTLE);  // Minimum throttle to arm the ESC
    delay(2000);  // Wait for arming sequence

    // Send zero throttle value to complete arming sequence
    Serial.println("Sending zero throttle to complete the arming sequence...");
    ESC.writeMicroseconds(ESC_MIN_THROTTLE);  // Zero throttle
    delay(1000);  // Allow ESC to register the zero throttle

    // Send zero throttle again (or fail-safe throttle if desired)
    Serial.println("Sending zero throttle again.");
    ESC.writeMicroseconds(ESC_MIN_THROTTLE);  // Zero throttle again
}

void disarmESC(Servo ESC) {
    // Send zero throttle value to disarm the ESC
    Serial.println("Disarming ESC...");
    ESC.writeMicroseconds(ESC_MIN_THROTTLE);  // Zero throttle
    delay(1000);  // Allow ESC to register the zero throttle
}

void setESC(int ESCPin, float amount) {
    int throttle = map(amount, 0, 100, ESC_MIN_THROTTLE, ESC_MAX_THROTTLE);
    switch (ESCPin) {
        case ESC_ONE_PIN:
            ESCOne.writeMicroseconds(throttle);
            break;
        case ESC_TWO_PIN:
            ESCTwo.writeMicroseconds(throttle);
            break;
        case ESC_THREE_PIN:
            ESCThree.writeMicroseconds(throttle);
            break;
        case ESC_FOUR_PIN:
            ESCFour.writeMicroseconds(throttle);
            break;
        default:
            break;
    }
}