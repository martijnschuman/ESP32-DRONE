//ESC.cpp

#include "ESC.h"

Servo ESCOne;
Servo ESCTwo;
Servo ESCThree;
Servo ESCFour;


void setupESC(Servo& ESC, int ESC_PIN) {
    Serial.print("Setting up ESC pin ");
    Serial.println(ESC_PIN);
    ESC.setPeriodHertz(ESC_FREQUENCY);
    ESC.attach(ESC_PIN, ESC_MIN_THROTTLE, ESC_MAX_THROTTLE);
    Serial.print("ESC ");
    Serial.print(ESC_PIN);
    Serial.println(" setup complete.");
}

void armESC(Servo& ESC) {
    Serial.println("Arming ESC...");
    ESC.writeMicroseconds(ESC_MIN_THROTTLE);  // Send low throttle
    delay(6000);  // Wait for arming to complete
    Serial.println("ESC armed.");
}

void setESC(int ESCPin, float amount) {
    int throttle = map(amount, 0, 100, ESC_MIN_THROTTLE, ESC_ALLOWED_MAX_THROTTLE);
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