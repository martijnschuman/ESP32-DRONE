#include "camera.h"

unsigned long lastAlivePulse = 0; // Last time the status pin was checked

void setupCamera() {
    pinMode(CAMERA_TRIGGER_PIN, OUTPUT);
    pinMode(CAMERA_RECEIVE_STATUS_PIN, INPUT);
    // Set trigger pin HIGH by default
    digitalWrite(CAMERA_TRIGGER_PIN, HIGH);
}

// Function to send trigger signal to the camera
void sendTrigger() {
    digitalWrite(CAMERA_TRIGGER_PIN, LOW);
    delay(50);
    digitalWrite(CAMERA_TRIGGER_PIN, HIGH);
    delay(50);
    digitalWrite(CAMERA_TRIGGER_PIN, LOW);
    delay(50);
    digitalWrite(CAMERA_TRIGGER_PIN, HIGH);

    Serial.println("Trigger signal sent.");
}

// Function to wait for picture confirmation
bool waitForConfirmation() {
    unsigned long startTime = millis();
    int pulseCount = 0;

    while (millis() - startTime < CAMERA_TIMEOUT) {
        // Read the status pin
        int status = digitalRead(CAMERA_RECEIVE_STATUS_PIN);

        if (status == LOW) { 
            // Detect a low pulse
            delay(50); // Wait for the pulse duration
            if (digitalRead(CAMERA_RECEIVE_STATUS_PIN) == HIGH) {
                pulseCount++;
                Serial.println("Pulse detected.");
            }
        }

        // Check if x (CAMERA_CONFIRMATION_PULSE_COUNT) pulses are detected
        if (pulseCount == CAMERA_CONFIRMATION_PULSE_COUNT) {
            Serial.println("Picture saved confirmation received.");
            return true;
        }
    }

    // Timeout reached without confirmation
    return false;
}

// Function to read status
void readStatus() {
    // Read the digital state of the status pin
    int status = digitalRead(CAMERA_RECEIVE_STATUS_PIN);

    if (status == HIGH) {
        Serial.println("Status: READY");
    } else {
        Serial.println("Status: ERROR");
    }
}

// Function to check if the camera is alive
void isCameraAlive() {
    unsigned long currentMillis = millis();

    // Send a pulse every CAMERA_KEEP_ALIVE_INTERVAL (e.g., 5 seconds)
    if (currentMillis - lastAlivePulse >= CAMERA_KEEP_ALIVE_INTERVAL) {
        lastAlivePulse = currentMillis;

        // Send is-alive pulse
        digitalWrite(CAMERA_TRIGGER_PIN, LOW);
        delay(10); // Brief low pulse
        digitalWrite(CAMERA_TRIGGER_PIN, HIGH);

        // Wait for response within CAMERA_KEEP_ALIVE_TIMEOUT (e.g., 100 ms)
        unsigned long responseStartTime = millis();
        bool responseReceived = false;

        while (millis() - responseStartTime < CAMERA_KEEP_ALIVE_TIMEOUT) {
            if (digitalRead(CAMERA_RECEIVE_STATUS_PIN) == HIGH) {
                responseReceived = true;
                Serial.println("Camera is alive.");
                break;
            }
        }

        // Log error if no response received
        if (!responseReceived) {
            Serial.println("Error: No response from camera or response timeout!");
        }
    }
}
