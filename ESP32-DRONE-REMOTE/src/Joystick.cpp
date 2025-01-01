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

// Generic calibration method for both joysticks
bool calibrateSingleJoystick(int &minX, int &maxX, int &minY, int &maxY, int &centerX, int &centerY, int pinX, int pinY, const char* joystickName) {
    Serial.print("Calibrating "); Serial.println(joystickName);
    displayLCD("Calibrating", 0, 0);
    displayLCD(joystickName, 1, 0);

    delay(LCD_REFRESH_INTERVAL);

    // Min X calibration
    Serial.println("Move the joystick to the left (min X) and hold for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds.");
    displayLCD("Move left", 0, 0);
    displayLCD(("for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds").c_str(), 1, 0);

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
    Serial.println("Now move the joystick to the right (max X) and hold for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds.");
    displayLCD("Move right", 0, 0);
    displayLCD(("for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds").c_str(), 1, 0);

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
    Serial.println("Move the joystick to the bottom (min Y) and hold for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds.");
    displayLCD("Move to bottom", 0, 0);
    displayLCD(("for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds").c_str(), 1, 0);

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
    Serial.println("Now move the joystick to the top (max Y) and hold for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds.");
    displayLCD("Move to top", 0, 0);
    displayLCD(("for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds").c_str(), 1, 0);

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
    displayLCD("Move to center", 0, 0);
    displayLCD(("for " + String(JOYSTICK_CALIBRATION_DELAY / 1000 * 2) + " seconds").c_str(), 1, 0);

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

    displayLCD(joystickName, 0, 0);
    displayLCD("complete", 1, 0);
    delay(LCD_REFRESH_INTERVAL);

    return true;
}
