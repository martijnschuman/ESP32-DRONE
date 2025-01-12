#include <joystick.h>

// Calibration data for left joystick
float leftMeasuredMinX = 0, leftMeasuredMaxX = 3300.00, leftMeasuredCenterX = leftMeasuredMinX / 2;
float leftMeasuredMinY = 3300.00, leftMeasuredMaxY = 0, leftMeasuredCenterY = leftMeasuredMinY / 2;
bool leftCalibrated = false;

// Calibration data for right joystick
float rightMeasuredMinX = 0, rightMeasuredMaxX = 3300.00, rightMeasuredCenterX = rightMeasuredMinX / 2;
float rightMeasuredMinY = 3300.00, rightMeasuredMaxY = 0, rightMeasuredCenterY = rightMeasuredMinY / 2;
bool rightCalibrated = false;


void setupJoysticks() {
    pinMode(JOYSTICK_LEFT_BUTTON, INPUT_PULLUP);
    pinMode(JOYSTICK_RIGHT_BUTTON, INPUT_PULLUP);
}

bool getLeftJoystickButton() {
    static unsigned long lastPressTime = 0;
    unsigned long currentTime = millis();

    if (digitalRead(JOYSTICK_LEFT_BUTTON) == LOW && (currentTime - lastPressTime) > DEBOUNCE_DELAY) {
        lastPressTime = currentTime;
        return true;
    }
    return false;
}

bool getRightJoystickButton() {
    static unsigned long lastPressTime = 0;
    unsigned long currentTime = millis();

    if (digitalRead(JOYSTICK_RIGHT_BUTTON) == LOW && (currentTime - lastPressTime) > DEBOUNCE_DELAY) {
        lastPressTime = currentTime;
        return true;
    }
    return false;
}

// Functions for reading joystick values
int getLeftJoystickX() {
    return transferJoystickValue(readADCChannel(JOYSTICK_LEFT_X_CHANNEL), leftMeasuredMinX, leftMeasuredMaxX, leftMeasuredCenterX, false);
}

int getLeftJoystickY() {
    return transferJoystickValue(readADCChannel(JOYSTICK_LEFT_Y_CHANNEL), leftMeasuredMinY, leftMeasuredMaxY, leftMeasuredCenterY, true);
}

int getRightJoystickX() {
    return transferJoystickValue(readADCChannel(JOYSTICK_RIGHT_X_CHANNEL), rightMeasuredMinX, rightMeasuredMaxX, rightMeasuredCenterX, false);
}

int getRightJoystickY() {
    return transferJoystickValue(readADCChannel(JOYSTICK_RIGHT_Y_CHANNEL), rightMeasuredMinY, rightMeasuredMaxY, rightMeasuredCenterY, true);
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
    displayLEDErrorStatus();

    leftCalibrated = calibrateSingleJoystick(leftMeasuredMinX, leftMeasuredMaxX, leftMeasuredMinY, leftMeasuredMaxY, leftMeasuredCenterX, leftMeasuredCenterY, JOYSTICK_LEFT_X_CHANNEL, JOYSTICK_LEFT_Y_CHANNEL, "left");
    rightCalibrated = calibrateSingleJoystick(rightMeasuredMinX, rightMeasuredMaxX, rightMeasuredMinY, rightMeasuredMaxY, rightMeasuredCenterX, rightMeasuredCenterY, JOYSTICK_RIGHT_X_CHANNEL, JOYSTICK_RIGHT_Y_CHANNEL, "right");

    displayLEDBlankStatus();
}

// Function to calibrate a single joystick
bool calibrateSingleJoystick(float &minX, float &maxX, float &minY, float &maxY, float &centerX, float &centerY, int channelX, int channelY, const char* joystickName) {
    Serial.print("Calibrating "); Serial.println(joystickName);

    // Bottom-Left Calibration
    Serial.println("Move the joystick to the bottom-left corner and hold.");
    displayCalibration(0, joystickName);

    delay(JOYSTICK_CALIBRATION_DELAY*(5/3)); // Wait for user action
    unsigned long startTime = millis();
    float sumX = 0, sumY = 0;
    int count = 0;

    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumX += readADCChannel(channelX);
        sumY += readADCChannel(channelY);
        count++;
    }

    minX = sumX / count;
    minY = sumY / count;
    Serial.print("Min X: "); Serial.println(minX);
    Serial.print("Min Y: "); Serial.println(minY);

    // Top-Right Calibration
    Serial.println("Move the joystick to the top-right corner and hold.");
    displayCalibration(1, joystickName);

    delay(JOYSTICK_CALIBRATION_DELAY*(5/3)); // Wait for user action
    startTime = millis();
    sumX = 0; sumY = 0; count = 0;

    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        sumX += readADCChannel(channelX);
        sumY += readADCChannel(channelY);
        count++;
    }

    maxX = sumX / count;
    maxY = sumY / count;
    Serial.print("Max X: "); Serial.println(maxX);
    Serial.print("Max Y: "); Serial.println(maxY);

    // Center Calibration
    Serial.println("Release the joystick to its center position.");
    displayCalibration(2, joystickName);

    delay(JOYSTICK_CALIBRATION_DELAY*(5/3)); // Wait for user action
    startTime = millis();
    float centerSumX = 0, centerSumY = 0;
    count = 0;

    while (millis() - startTime < JOYSTICK_CALIBRATION_DELAY) {
        centerSumX += readADCChannel(channelX);
        centerSumY += readADCChannel(channelY);
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

    displayCalibration(3, joystickName);

    return true;
}
