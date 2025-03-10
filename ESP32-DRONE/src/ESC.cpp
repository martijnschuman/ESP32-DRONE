//ESC.cpp

#include "ESC.h"

Servo ESCOne;
Servo ESCTwo;
Servo ESCThree;
Servo ESCFour;

bool isESCOneArmed = false;
bool isESCTwoArmed = false;
bool isESCThreeArmed = false;
bool isESCFourArmed = false;

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

bool armAllESCs(){
    Serial.println("Starting ESC arming...");

    delay(4000);

    if (!isESCOneArmed) {
        setupESC(ESCOne, ESC_ONE_PIN);
        delay(1000);
        armESC(ESCOne);
        isESCOneArmed = true;
    }
    if (!isESCTwoArmed) {
        setupESC(ESCTwo, ESC_TWO_PIN);
        delay(1000);
        armESC(ESCTwo);
        isESCTwoArmed = true;
    }
    if (!isESCThreeArmed) {
        setupESC(ESCThree, ESC_THREE_PIN);
        delay(1000);
        armESC(ESCThree);
        isESCThreeArmed = true;
    }
    if (!isESCFourArmed) {
        setupESC(ESCFour, ESC_FOUR_PIN);
        delay(1000);
        armESC(ESCFour);
        isESCFourArmed = true;
    }

    return true;
}

void setESC(int ESCPin, float amount) {
    // Map the throttle value from 0-100 to ESC_MIN_THROTTLE to ESC_ALLOWED_MAX_THROTTLE
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

void applyMotorAdjustments(float rollAdjust, float pitchAdjust, float yawAdjust) {
    // Adjust motor speeds based on your custom mapping
    float esc1 = controlPacket.throttle - pitchAdjust - rollAdjust + yawAdjust;  // Back Left (ESC1)
    float esc2 = controlPacket.throttle + pitchAdjust - rollAdjust - yawAdjust;  // Front Left (ESC2)
    float esc3 = controlPacket.throttle - pitchAdjust + rollAdjust - yawAdjust;  // Back Right (ESC3)
    float esc4 = controlPacket.throttle + pitchAdjust + rollAdjust + yawAdjust;   // Front Right (ESC4)

    // Constrain outputs to valid range (0 to 100)
    esc1 = constrain(esc1, 0, 100);
    esc2 = constrain(esc2, 0, 100);
    esc3 = constrain(esc3, 0, 100);
    esc4 = constrain(esc4, 0, 100);

    // Set ESC signals
    setESC(ESC_ONE_PIN, esc1);
    setESC(ESC_TWO_PIN, esc2);
    setESC(ESC_THREE_PIN, esc3);
    setESC(ESC_FOUR_PIN, esc4);
}
