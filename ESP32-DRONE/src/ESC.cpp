//ESC.cpp

#include "ESC.h"

Servo ESCOne, ESCTwo, ESCThree, ESCFour;
bool isESCOneArmed, isESCTwoArmed, isESCThreeArmed, isESCFourArmed = false;
float ESCONE_THROTTLE, ESCTWO_THROTTLE, ESCTHREE_THROTTLE, ESCFOUR_THROTTLE = 0;

void setupESC(Servo& ESC, int ESC_PIN) {
    Serial.printf("Setting up ESC pin %d\n", ESC_PIN);

    ESC.setPeriodHertz(ESC_FREQUENCY);
    ESC.attach(ESC_PIN, ESC_MIN_THROTTLE_MS, ESC_MAX_THROTTLE_MS);

    Serial.printf("ESC %d setup complete\n", ESC_PIN);
}

void armESC(Servo& ESC, int ESC_PIN) {
	Serial.printf("Arming ESC on pin %d\n", ESC_PIN);
    ESC.writeMicroseconds(ESC_MIN_THROTTLE_MS);  // Send low throttle
    delay(4500);  // Wait for arming to complete
    Serial.println("ESC armed.");
}

bool armAllESCs(){
    Serial.println("Starting ESC arming...");

    if (!isESCOneArmed) {
        setupESC(ESCOne, ESC_ONE_PIN);
        delay(1000);
        armESC(ESCOne, ESC_ONE_PIN);
        isESCOneArmed = true;
    }
	delay(500);
    if (!isESCTwoArmed) {
        setupESC(ESCTwo, ESC_TWO_PIN);
        delay(1000);
        armESC(ESCTwo, ESC_TWO_PIN);
        isESCTwoArmed = true;
    }
	delay(500);
    if (!isESCThreeArmed) {
        setupESC(ESCThree, ESC_THREE_PIN);
        delay(1000);
        armESC(ESCThree, ESC_THREE_PIN);
        isESCThreeArmed = true;
    }
	delay(500);
    if (!isESCFourArmed) {
        setupESC(ESCFour, ESC_FOUR_PIN);
        delay(1000);
        armESC(ESCFour, ESC_FOUR_PIN);
        isESCFourArmed = true;
    }

	Serial.println("All ESCs armed successfully.");
    return true;
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Improved setESC that avoids sending a zero-throttle command during flight.
void setESC(int escPin, float amount) {
	// Prevent sending zero throttle during flight (unless in emergency descent)
	if (amount <= 0 && getFlightMode() != EMERGENCY_DECENT) {
		return;
	}

	// Map amount (0-100) to pulse width range [ESC_MIN_THROTTLE_MS, ESC_ALLOWED_MAX_THROTTLE_MS]
	int throttleVal = (int) mapFloat(amount, 0, 100, ESC_MIN_THROTTLE_MS, ESC_ALLOWED_MAX_THROTTLE_MS);
	if (throttleVal < ESC_MIN_THROTTLE_MS) throttleVal = ESC_MIN_THROTTLE_MS;
	if (throttleVal > ESC_ALLOWED_MAX_THROTTLE_MS) throttleVal = ESC_ALLOWED_MAX_THROTTLE_MS;

	// Write the value to the appropriate ESC (via its Servo object)
	if (escPin == ESC_ONE_PIN) {
		ESCOne.writeMicroseconds(throttleVal);
	} else if (escPin == ESC_TWO_PIN) {
		ESCTwo.writeMicroseconds(throttleVal);
	} else if (escPin == ESC_THREE_PIN) {
		ESCThree.writeMicroseconds(throttleVal);
	} else if (escPin == ESC_FOUR_PIN) {
		ESCFour.writeMicroseconds(throttleVal);
	}
}

// --- Improved Mixing Function ---
// Computes motor outputs based on an elongated X configuration.
// Motor assignments (assuming the following orientation):
//   • ESC1 (Back Left):  base + pitchAdj + rollAdj + yawAdj
//   • ESC2 (Front Left): base - pitchAdj + rollAdj - yawAdj
//   • ESC3 (Back Right): base + pitchAdj - rollAdj - yawAdj
//   • ESC4 (Front Right): base - pitchAdj - rollAdj + yawAdj
void applyMotorAdjustments(float pitchAdj, float rollAdj, float yawAdj) {
	float baseThrottle = controlPacket.throttle;

	// If throttle is zero (and not in emergency descent), do nothing.
	if (baseThrottle <= 0 && getFlightMode() != EMERGENCY_DECENT) {
		return;
	}

	// Compute raw motor outputs
	float motor1 = baseThrottle + pitchAdj + rollAdj + yawAdj;
	float motor2 = baseThrottle - pitchAdj + rollAdj - yawAdj;
	float motor3 = baseThrottle + pitchAdj - rollAdj - yawAdj;
	float motor4 = baseThrottle - pitchAdj - rollAdj + yawAdj;

	// Prevent motor saturation and ensure minimum output (Air Mode)
    // This logic ensures we maintain control authority even at throttle extremes.
    // If throttle is active, ensure no motor drops below the minimum required output.
    // This is critical for maintaining stability during maneuvers at low throttle.
    if (baseThrottle > 0 && getFlightMode() != EMERGENCY_DECENT) {
        float minMotor = min(min(motor1, motor2), min(motor3, motor4));

        if (minMotor < MIN_MOTOR_OUTPUT_PERCENTAGE) {
            // This offset will bring the lowest motor up to the minimum.
            float offset = MIN_MOTOR_OUTPUT_PERCENTAGE - minMotor;

            float maxMotor = max(max(motor1, motor2), max(motor3, motor4));
            if (maxMotor + offset > 100) {
                // Reduce the offset to prevent saturation.
                offset = 100 - maxMotor;
            }

            // Apply the calculated (and possibly reduced) offset to all motors.
            // This preserves the relative differences in motor speeds, maintaining control.
            motor1 += offset;
            motor2 += offset;
            motor3 += offset;
            motor4 += offset;
        }
    }

	// Final constrain to be sure.
	motor1 = constrain(motor1, 0, 100);
	motor2 = constrain(motor2, 0, 100);
	motor3 = constrain(motor3, 0, 100);
	motor4 = constrain(motor4, 0, 100);

	ESCONE_THROTTLE = motor1;
	ESCTWO_THROTTLE = motor2;
	ESCTHREE_THROTTLE = motor3;
	ESCFOUR_THROTTLE = motor4;

	// Output to ESCs.
	setESC(ESC_ONE_PIN, motor1);
	setESC(ESC_TWO_PIN, motor2);
	setESC(ESC_THREE_PIN, motor3);
	setESC(ESC_FOUR_PIN, motor4);

	hasBeenThrottled = true;
}

void emergencyDescent() {
    static unsigned long lastDescentUpdate = 0;
    const unsigned long descentInterval = 200;      // milliseconds between throttle decrements
    const float throttleStep = 5;                   // throttle decrement step

    // Check if it's time to update the throttle
    if (millis() - lastDescentUpdate >= descentInterval) {
        lastDescentUpdate = millis();

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
    applyMotorAdjustments(0, 0, 0);
}
