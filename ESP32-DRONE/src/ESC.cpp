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

float ESCONE_THROTTLE = 0;
float ESCTWO_THROTTLE = 0;
float ESCTHREE_THROTTLE = 0;
float ESCFOUR_THROTTLE = 0;

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

    delay(2000);

    if (!isESCOneArmed) {
        setupESC(ESCOne, ESC_ONE_PIN);
        delay(1000);
        armESC(ESCOne);
        isESCOneArmed = true;
    }
	delay(500);
    if (!isESCTwoArmed) {
        setupESC(ESCTwo, ESC_TWO_PIN);
        delay(1000);
        armESC(ESCTwo);
        isESCTwoArmed = true;
    }
	delay(500);
    if (!isESCThreeArmed) {
        setupESC(ESCThree, ESC_THREE_PIN);
        delay(1000);
        armESC(ESCThree);
        isESCThreeArmed = true;
    }
	delay(500);
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

// Rate limiter for any control input
float rateLimit(float currentValue, float desiredValue, float maxDelta) {
    return currentValue + constrain(desiredValue - currentValue, -maxDelta, maxDelta);
}

void applyMotorAdjustments(float rollAdjust, float pitchAdjust, float yawAdjust) {   
    // Use static variables to hold the current, rate-limited control values
    static float currentThrottle = 0.0f;
    static float currentRoll = 0.0f;
    static float currentPitch = 0.0f;
    static float currentYaw = 0.0f;
    
    // Define maximum change per update cycle for each channel
    const float throttleMaxDelta = 2.0f;	// Throttle change per cycle
    const float attitudeMaxDelta = 5.0f;	// Roll, pitch, yaw change per cycle

    // Rate limit the channels
    currentThrottle = rateLimit(currentThrottle, controlPacket.throttle, throttleMaxDelta);
    currentRoll     = rateLimit(currentRoll, rollAdjust, attitudeMaxDelta);
    currentPitch    = rateLimit(currentPitch, pitchAdjust, attitudeMaxDelta);
    currentYaw      = rateLimit(currentYaw, yawAdjust, attitudeMaxDelta);
    
    // Scale yaw adjustments to balance the motor mix
    const float yawScale = 0.25f;  // Adjust this value to fine-tune yaw effect
	const float pitchScale = 0.25f;  // Adjust this value to fine-tune pitch effect

    // Mix the rate-limited commands into motor outputs
    // The following mixing equation maps the control inputs to each ESC.
    ESCONE_THROTTLE = currentThrottle - (currentPitch * pitchScale) - currentRoll + (currentYaw * yawScale);  // Back Left (ESC1)
    ESCTWO_THROTTLE = currentThrottle + (currentPitch * pitchScale) - currentRoll - (currentYaw * yawScale);  // Front Left (ESC2)
    ESCTHREE_THROTTLE = currentThrottle - (currentPitch * pitchScale) + currentRoll - (currentYaw * yawScale);  // Back Right (ESC3)
    ESCFOUR_THROTTLE = currentThrottle + (currentPitch * pitchScale) + currentRoll + (currentYaw * yawScale);  // Front Right (ESC4)

    // Constrain outputs to a valid range (0 to 100)
	float minCommand = (controlPacket.throttle > 0) ? 5 : 0;
    ESCONE_THROTTLE = constrain(ESCONE_THROTTLE, minCommand, 100);
    ESCTWO_THROTTLE = constrain(ESCTWO_THROTTLE, minCommand, 100);
    ESCTHREE_THROTTLE = constrain(ESCTHREE_THROTTLE, minCommand, 100);
    ESCFOUR_THROTTLE = constrain(ESCFOUR_THROTTLE, minCommand, 100);

	// If throttle is zero, set all ESCs to zero
	if(controlPacket.throttle == 0) {
		ESCONE_THROTTLE = 0;
		ESCTWO_THROTTLE = 0;
		ESCTHREE_THROTTLE = 0;
		ESCFOUR_THROTTLE = 0;
	}

    // Send commands to each ESC
    setESC(ESC_ONE_PIN, ESCONE_THROTTLE);
    setESC(ESC_TWO_PIN, ESCTWO_THROTTLE);
    setESC(ESC_THREE_PIN, ESCTHREE_THROTTLE);
    setESC(ESC_FOUR_PIN, ESCFOUR_THROTTLE);
}

void emergencyDescent() {
    static unsigned long lastDescentUpdate = 0;
    const unsigned long descentInterval = 200; // milliseconds between throttle decrements
    const float throttleStep = 5;             // throttle decrement step

    // Check if it's time to update the throttle (avoid updating every loop iteration)
    if (millis() - lastDescentUpdate >= descentInterval) {
        lastDescentUpdate = millis();

        // Assuming controlPacket.throttle holds the current throttle value (scale 0-100)
        if (height > 5) {
            // Gradually reduce throttle, but never below 20 while descending
            if (controlPacket.throttle > throttleStep) {
                controlPacket.throttle = max(controlPacket.throttle - throttleStep, throttleStep);
                Serial.print("Emergency descent throttle: ");
                Serial.println(controlPacket.throttle);
            }
        } else {
            // When the drone is near the ground, cut throttle instantly
            controlPacket.throttle = 0;
            Serial.println("Emergency descent complete: Throttle set to 0");
        }
    }

    // Apply the controlled throttle with zero corrections on roll, pitch, and yaw for balanced power-off.
    // Optionally, you can disable PID adjustments here.
    applyMotorAdjustments(0, 0, 0);
}
