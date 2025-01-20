// status.cpp
#include "status.h"

unsigned long currentMillis = 0;            // Stores the current time
unsigned long previousMillis = 0;           // Stores the last time the LED was updated

unsigned long blinkStartMillis = 0;         // Time when the current blink started
bool ledState = LOW;                        // State of the LED (LOW = OFF, HIGH = ON)
int blinkStep = 0;                          // Step for blink pattern (0 for on, 1 for off)


// Observer function to set the status
void setStatus(CameraMode newStatus) {
    status = newStatus;
    Serial.print("Status changed to: ");
    Serial.println(status);
    reportStatus();
}

// Function to report status
void reportStatus() {
    switch (status) {
        case CAM_READY:
            showLEDStatus(LED_READY);
            break;
        case CAM_PICTURE_SAVED:
            showLEDStatus(LED_FILE_SAVED);
            break;
        case SD_INIT_ERROR:
        case SD_SAVE_ERROR:
        case CAM_INIT_ERROR:
        case CAMERA_TAKE_ERROR:
            showLEDStatus(LED_SYSTEM_ERROR);
            break;
        case CAM_ESP_NOW_INIT_ERROR:
        case CAM_ESP_NOW_SEND_ERROR:
            showLEDStatus(LED_ESP_NOW_ERROR);
            break;
    }
}

// Function to control the LED based on the status
void showLEDStatus(LEDStatusEnum ledStatus) {
    currentMillis = millis();  // Get the current time

    switch (ledStatus) {
        case LED_READY:
            // The LED is always ON for the "Ready" state
            digitalWrite(STATUS_LED_PIN, LOW);  // Inverted logic (LOW is ON)
            break;

        case LED_SYSTEM_ERROR:
            // Error: blink pattern (150 ms ON, 50 ms OFF, repeat)
            if (currentMillis - blinkStartMillis >= (blinkStep == 0 ? SYSTEM_ERROR_LED_ON_DURATION : SYSTEM_ERROR_LED_OFF_DURATION)) {
                // Toggle LED state
                ledState = (blinkStep == 0) ? LOW : HIGH;  // ON for 150 ms, OFF for 50 ms
                digitalWrite(STATUS_LED_PIN, ledState);

                // Move to the next blink step (0 = ON, 1 = OFF)
                blinkStep = (blinkStep + 1) % 2;  // Alternate between 0 and 1

                blinkStartMillis = currentMillis;
            }
            break;

        case LED_ESP_NOW_ERROR:
            // ESP-NOW error: fast blink pattern (500 ms ON, 50 ms OFF, repeat)
            if (currentMillis - blinkStartMillis >= (blinkStep == 0 ? ESP_NOW_ERROR_LED_ON_DURATION : ESP_NOW_ERROR_LED_OFF_DURATION)) {
                // Toggle LED state
                ledState = (blinkStep == 0) ? LOW : HIGH;  // ON for 500 ms, OFF for 50 ms
                digitalWrite(STATUS_LED_PIN, ledState);

                // Move to the next blink step (0 = ON, 1 = OFF)
                blinkStep = (blinkStep + 1) % 2;  // Alternate between 0 and 1

                // Reset the blink start time
                blinkStartMillis = currentMillis;
            }
            break;
        
        case LED_FILE_SAVED:
            digitalWrite(STATUS_LED_PIN, HIGH);  // FILE_SAVED: Turn LED off
            for (int i = 0; i < FILE_SAVE_BLINK_COUNT; i++) {
                digitalWrite(STATUS_LED_PIN, LOW);
                delay(50);
                digitalWrite(STATUS_LED_PIN, HIGH);
                delay(50);
            }
            break;
    }
}
