//ESC.cpp

#include "ESC.h"

Servo ESCOne, ESCTwo, ESCThree, ESCFour;
bool isESCOneArmed, isESCTwoArmed, isESCThreeArmed, isESCFourArmed = false;
float ESCONE_THROTTLE, ESCTWO_THROTTLE, ESCTHREE_THROTTLE, ESCFOUR_THROTTLE = 0;

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
    delay(4500);  // Wait for arming to complete
    Serial.println("ESC armed.");
}

bool armAllESCs(){
    Serial.println("Starting ESC arming...");

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

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Improved setESC that avoids sending a zero-throttle command during flight.
void setESC(int escPin, float amount) {
	// Prevent sending zero throttle during flight (unless in emergency descent)
	if (amount <= 0 && getFlightMode() != EMERGENCY_DECENT) {
		return;
	}

	// Map amount (0-100) to pulse width range [ESC_MIN_THROTTLE, ESC_ALLOWED_MAX_THROTTLE]
	int throttleVal = (int) mapFloat(amount, 0, 100, ESC_MIN_THROTTLE, ESC_ALLOWED_MAX_THROTTLE);
	if (throttleVal < ESC_MIN_THROTTLE) throttleVal = ESC_MIN_THROTTLE;
	if (throttleVal > ESC_ALLOWED_MAX_THROTTLE) throttleVal = ESC_ALLOWED_MAX_THROTTLE;

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

	// Constrain each output to the 0–100 range.
	motor1 = constrain(motor1, 0, 100);
	motor2 = constrain(motor2, 0, 100);
	motor3 = constrain(motor3, 0, 100);
	motor4 = constrain(motor4, 0, 100);

	// Determine the smallest motor value.
	float minMotor = motor1;
	if (motor2 < minMotor) minMotor = motor2;
	if (motor3 < minMotor) minMotor = motor3;
	if (motor4 < minMotor) minMotor = motor4;

	// If any motor falls below the minimum output (and we're flying normally),
	// add an offset to all motors (if possible) to raise the minimum.
	if (baseThrottle > 0 && getFlightMode() != EMERGENCY_DECENT && minMotor < MIN_MOTOR_OUTPUT) {
		float offset = MIN_MOTOR_OUTPUT - minMotor;

		// Check that adding offset does not push any motor above 100%.
		float maxMotor = motor1;
		if (motor2 > maxMotor) maxMotor = motor2;
		if (motor3 > maxMotor) maxMotor = motor3;
		if (motor4 > maxMotor) maxMotor = motor4;
		if (maxMotor + offset <= 100) {
			motor1 += offset;
			motor2 += offset;
			motor3 += offset;
			motor4 += offset;
		} else {
			// Otherwise, clamp motors individually.
			if (motor1 < MIN_MOTOR_OUTPUT) motor1 = MIN_MOTOR_OUTPUT;
			if (motor2 < MIN_MOTOR_OUTPUT) motor2 = MIN_MOTOR_OUTPUT;
			if (motor3 < MIN_MOTOR_OUTPUT) motor3 = MIN_MOTOR_OUTPUT;
			if (motor4 < MIN_MOTOR_OUTPUT) motor4 = MIN_MOTOR_OUTPUT;
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

    // Check if it's time to update the throttle (avoid updating every loop iteration)
    if (millis() - lastDescentUpdate >= descentInterval) {
        lastDescentUpdate = millis();

        // Assuming controlPacket.throttle holds the current throttle value (scale 0-100)
        if (height > 5) {
            // Gradually reduce throttle, but never below 20 while descending
            if (controlPacket.throttle > throttleStep) {
                controlPacket.throttle = max(controlPacket.throttle - throttleStep, throttleStep);
                Serial.print("Emergency descent throttle: ");
                // Serial.println(controlPacket.throttle);
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
