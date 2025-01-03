#include <joystick.h>

// Calibration data for left joystick
int leftMeasuredMinX = 4095, leftMeasuredMaxX = 0, leftMeasuredCenterX = leftMeasuredMinX / 2;
int leftMeasuredMinY = 4095, leftMeasuredMaxY = 0, leftMeasuredCenterY = leftMeasuredMinY / 2;
bool leftCalibrated = false;

// Calibration data for right joystick
int rightMeasuredMinX = 4095, rightMeasuredMaxX = 0, rightMeasuredCenterX = rightMeasuredMinX / 2;
int rightMeasuredMinY = 4095, rightMeasuredMaxY = 0, rightMeasuredCenterY = rightMeasuredMinY / 2;
bool rightCalibrated = false;

// Setup function for joysticks
void setupJoysticks() {
    pinMode(LEFT_VRX, INPUT);
    pinMode(LEFT_VRY, INPUT);

    pinMode(RIGHT_VRX, INPUT);
    pinMode(RIGHT_VRY, INPUT);
}

// Functions for reading joystick values
int getLeftJoystickX() {
    return transferJoystickValue(analogRead(LEFT_VRX), leftMeasuredMinX, leftMeasuredMaxX, leftMeasuredCenterX, false);
}

int getLeftJoystickY() {
    return transferJoystickValue(analogRead(LEFT_VRY), leftMeasuredMinY, leftMeasuredMaxY, leftMeasuredCenterY, true);
}

int getRightJoystickX() {
    return transferJoystickValue(analogRead(RIGHT_VRX), rightMeasuredMinX, rightMeasuredMaxX, rightMeasuredCenterX, false);
}

int getRightJoystickY() {
    return transferJoystickValue(analogRead(RIGHT_VRY), rightMeasuredMinY, rightMeasuredMaxY, rightMeasuredCenterY, true);
}

// Transfer function to scale joystick values with two separate mappings and dead zone
int transferJoystickValue(int value, int measuredMinValue, int measuredMaxValue, int centerValue, bool isYAxis) {
    int result;

    // Define a dead zone around the center value
    int deadZone = JOYSTICK_CENTER_TOLERANCE;

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
        result = result + 12;
    } else {
        // X-axis does not need special treatment
        if (value < centerValue) {
            result = map(value, measuredMinValue, centerValue, -100, 0);
        } else {
            result = map(value, centerValue, measuredMaxValue, 0, 100);
        }
    }

    if (result > 100) {
        result = 100;
    } else if (result < -100) {
        result = -100;
    }

    return result;
}

void startCalibrateJoysticks() {
    displayErrorStatus();

    leftCalibrated = calibrateSingleJoystick(leftMeasuredMinX, leftMeasuredMaxX, leftMeasuredMinY, leftMeasuredMaxY, leftMeasuredCenterX, leftMeasuredCenterY, LEFT_VRX, LEFT_VRY, "Left joystick");
    rightCalibrated = calibrateSingleJoystick(rightMeasuredMinX, rightMeasuredMaxX, rightMeasuredMinY, rightMeasuredMaxY, rightMeasuredCenterX, rightMeasuredCenterY, RIGHT_VRX, RIGHT_VRY, "Right joystick");

    displayOKStatus();
    displayLCD("Calibration", 0, 0);
    displayLCD("complete!", 1, 0);

    delay(LCD_REFRESH_INTERVAL * 2); // Keep the green LED on for a short duration

    displayBlankStatus();
    clearLCD();
}

// Function to calibrate a single joystick
bool calibrateSingleJoystick(int &minX, int &maxX, int &minY, int &maxY, int &centerX, int &centerY, int pinX, int pinY, const char* joystickName) {
    Serial.print("Calibrating "); Serial.println(joystickName);
    displayLCD("Calibrating", 0, 0);
    displayLCD(joystickName, 1, 0);

    delay(LCD_REFRESH_INTERVAL);

    // Bottom-Left Calibration
    Serial.println("Move the joystick to the bottom-left corner and hold.");
    displayLCD("Move to", 0, 0);
    displayLCD("bottom-left", 1, 0);

    delay(JOYSTICK_CALIBRATION_DELAY); // Wait for user action
    unsigned long startTime = millis();
    long sumX = 0, sumY = 0;
    int count = 0;

    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumX += analogRead(pinX);
        sumY += analogRead(pinY);
        count++;
    }

    minX = sumX / count;
    minY = sumY / count;
    Serial.print("Min X: "); Serial.println(minX);
    Serial.print("Min Y: "); Serial.println(minY);

    // Top-Right Calibration
    Serial.println("Move the joystick to the top-right corner and hold.");
    displayLCD("Move to", 0, 0);
    displayLCD("top-right", 1, 0);

    delay(JOYSTICK_CALIBRATION_DELAY); // Wait for user action
    startTime = millis();
    sumX = 0; sumY = 0; count = 0;

    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumX += analogRead(pinX);
        sumY += analogRead(pinY);
        count++;
    }

    maxX = sumX / count;
    maxY = sumY / count;
    Serial.print("Max X: "); Serial.println(maxX);
    Serial.print("Max Y: "); Serial.println(maxY);

    // Center Calibration
    Serial.println("Release the joystick to its center position.");
    displayLCD("Move to", 0, 0);
    displayLCD("center", 1, 0);

    delay(JOYSTICK_CALIBRATION_DELAY); // Wait for joystick to settle
    startTime = millis();
    long centerSumX = 0, centerSumY = 0;
    count = 0;

    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        centerSumX += analogRead(pinX);
        centerSumY += analogRead(pinY);
        count++;
    }

    centerX = centerSumX / count;
    centerY = centerSumY / count;

    // Report Calibration Results
    Serial.println(joystickName + String(" calibration complete!"));
    Serial.print("Min X: "); Serial.println(minX);
    Serial.print("Max X: "); Serial.println(maxX);
    Serial.print("Min Y: "); Serial.println(minY);
    Serial.print("Max Y: "); Serial.println(maxY);
    Serial.print("Center X: "); Serial.println(centerX);
    Serial.print("Center Y: "); Serial.println(centerY);

    displayLCD(joystickName, 0, 0);
    displayLCD("Complete!", 1, 0);
    delay(LCD_REFRESH_INTERVAL);

    return true;
}
