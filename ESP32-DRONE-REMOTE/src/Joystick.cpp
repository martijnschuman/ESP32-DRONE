#include <Joystick.h>
#include <Config.h>

// Calibration data for left joystick
int leftJoystickMeasuredMinX = 4095, leftJoystickMeasuredMaxX = 0, leftJoystickMeasuredCenterX = 0;
int leftJoystickMeasuredMinY = 4095, leftJoystickMeasuredMaxY = 0, leftJoystickMeasuredCenterY = 0;
bool leftCalibrated = false;

// Setup function for joysticks
void setupJoysticks() {
    pinMode(LEFT_VRX, INPUT);
    pinMode(LEFT_VRY, INPUT);
    pinMode(RIGHT_VRX, INPUT);
    pinMode(RIGHT_VRY, INPUT);
    pinMode(START_CALIBRATION_BUTTON, INPUT_PULLUP);
}

// Generic calibration method for both joysticks
void calibrateJoystick(int &minX, int &maxX, int &centerX, int &minY, int &maxY, int &centerY, int pinX, int pinY, const char* joystickName) {
    Serial.print("Calibrating "); Serial.println(joystickName);

    // Min X calibration
    Serial.println("Move the joystick to the left (min X) and hold.");
    delay(JOYSTICK_CALIBRATION_DELAY);
    unsigned long startTime = millis();
    long sumX = 0;
    int countX = 0;
    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumX += analogRead(pinX);
        countX++;
    }
    minX = sumX / countX;
    Serial.print("Min X: ");
    Serial.println(minX);

    // Max X calibration
    Serial.println("Now move the joystick to the right (max X) and hold.");
    delay(JOYSTICK_CALIBRATION_DELAY);
    startTime = millis();
    sumX = 0;
    countX = 0;
    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumX += analogRead(pinX);
        countX++;
    }
    maxX = sumX / countX;
    Serial.print("Max X: ");
    Serial.println(maxX);

    // Min Y calibration
    Serial.println("Move the joystick to the bottom (min Y) and hold.");
    delay(JOYSTICK_CALIBRATION_DELAY);
    startTime = millis();
    long sumY = 0;
    int countY = 0;
    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumY += analogRead(pinY);
        countY++;
    }
    minY = sumY / countY;
    Serial.print("Min Y: ");
    Serial.println(minY);

    // Max Y calibration
    Serial.println("Now move the joystick to the top (max Y) and hold.");
    delay(JOYSTICK_CALIBRATION_DELAY);
    startTime = millis();
    sumY = 0;
    countY = 0;
    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumY += analogRead(pinY);
        countY++;
    }
    maxY = sumY / countY;
    Serial.print("Max Y: ");
    Serial.println(maxY);

    // Center calibration
    Serial.println("Release the joystick to its center position.");
    delay(JOYSTICK_CALIBRATION_DELAY);  // Wait for the joystick to settle
    long centerSumX = 0, centerSumY = 0;
    int count = 0;
    for (int i = 0; i < 1000; i++) {  // Taking 100 samples for center
        centerSumX += analogRead(pinX);
        centerSumY += analogRead(pinY);
        delay(10);  // Small delay to prevent reading too fast
        count++;
    }
    centerX = centerSumX / count;
    centerY = centerSumY / count;

    Serial.print(joystickName); Serial.println(" calibration complete!");
    Serial.print("Min X: "); Serial.println(minX);
    Serial.print("Max X: "); Serial.println(maxX);
    Serial.print("Min Y: "); Serial.println(minY);
    Serial.print("Max Y: "); Serial.println(maxY);
    Serial.print("Center X: "); Serial.println(centerX);
    Serial.print("Center Y: "); Serial.println(centerY);
}

// Transfer function to scale joystick values with two separate mappings and dead zone
int transferJoystickValue(int value, int measuredMinValue, int measuredMaxValue, int centerValue, bool isYAxis = false) {
    int result;

    // Define a dead zone around the center value (e.g., ±10)
    int deadZone = 10;

    // If the value is within the dead zone, return 0
    if (abs(value - centerValue) < deadZone) {
        return 0;
    }

    // Adjust the mapping for the Y-axis (since it's inverted)
    if (isYAxis) {
        // If the value is less than the center, scale it to -100 to 0 range
        if (value < centerValue) {
            result = map(value, measuredMinValue, centerValue, -100, 0);
        }
        // If the value is greater than the center, scale it to 0 to 100 range
        else {
            result = map(value, centerValue, measuredMaxValue, 0, 100);
        }

        // Adjust the Y-axis range slightly (correction factor)
        // We are applying a small correction factor here to address the misalignment
        result = result + 12;  // Adjust by 12 units for now; can be fine-tuned further

    } else {
        // X-axis does not need special treatment
        if (value < centerValue) {
            result = map(value, measuredMinValue, centerValue, -100, 0);
        } else {
            result = map(value, centerValue, measuredMaxValue, 0, 100);
        }
    }

    return result;
}


// Functions for reading joystick values
int transferredLeftJoystickReadX() {
    return transferJoystickValue(analogRead(LEFT_VRX), leftJoystickMeasuredMinX, leftJoystickMeasuredMaxX, leftJoystickMeasuredCenterX);
}

int transferredLeftJoystickReadY() {
    return transferJoystickValue(analogRead(LEFT_VRY), leftJoystickMeasuredMinY, leftJoystickMeasuredMaxY, leftJoystickMeasuredCenterY);
}

// Calibration trigger for the left joystick
void calibrateLeftJoystick() {
    calibrateJoystick(leftJoystickMeasuredMinX, leftJoystickMeasuredMaxX, leftJoystickMeasuredCenterX, leftJoystickMeasuredMinY, leftJoystickMeasuredMaxY, leftJoystickMeasuredCenterY, LEFT_VRX, LEFT_VRY, "Left joystick");
    leftCalibrated = true;
}

// Function to check if the left joystick is calibrated
bool getLeftCalibrated() {
    return leftCalibrated;
}

// Function to check the start calibration button
bool checkStartCalibrationButton() {
    static unsigned long lastPressTime = 0;
    unsigned long currentTime = millis();

    if (digitalRead(START_CALIBRATION_BUTTON) == LOW && (currentTime - lastPressTime) > DEBOUNCE_DELAY) {
        lastPressTime = currentTime;
        return true;
    }
    return false;
}
