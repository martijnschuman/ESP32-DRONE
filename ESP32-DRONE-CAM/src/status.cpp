// status.cpp
#include "status.h"

CommandStatusEnum status;                   // Set initial status
unsigned long currentMillis = 0;            // Stores the current time
unsigned long previousMillis = 0;           // Stores the last time the LED was updated

unsigned long blinkStartMillis = 0;         // Time when the current blink started
bool ledState = LOW;                        // State of the LED (LOW = OFF, HIGH = ON)
int blinkStep = 0;                          // Step for blink pattern (0 for on, 1 for off)


// Observer function to set the status
void setStatus(CommandStatusEnum newStatus) {
    status = newStatus;
    reportStatus();
    sendStatusToMaster();
}

// Function to report status
void reportStatus() {
    switch (status) {
        case READY:
            showLEDStatus(LED_READY);
            break;
        case FILE_SAVED:
            showLEDStatus(LED_FILE_SAVED);
            break;
        case SD_INIT_ERROR:
            showLEDStatus(LED_SYSTEM_ERROR);
            break;
        case SD_SAVE_ERROR:
            showLEDStatus(LED_SYSTEM_ERROR);
            break;
        case CAMERA_INIT_ERROR:
            showLEDStatus(LED_SYSTEM_ERROR);
            break;
        case CAMERA_TAKE_ERROR:
            showLEDStatus(LED_SYSTEM_ERROR);
            break;
        case ESP_NOW_INIT_ERROR:
            showLEDStatus(LED_ESP_NOW_ERROR);
            break;
        case ESP_NOW_ADD_PEER_ERROR:
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
                digitalWrite(STATUS_LED_PIN, ledState);  // Apply the LED state

                // Move to the next blink step (0 = ON, 1 = OFF)
                blinkStep = (blinkStep + 1) % 2;  // Alternate between 0 and 1

                // Reset the blink start time
                blinkStartMillis = currentMillis;
            }
            break;

        case LED_ESP_NOW_ERROR:
            // ESP-NOW error: fast blink pattern (500 ms ON, 50 ms OFF, repeat)
            if (currentMillis - blinkStartMillis >= (blinkStep == 0 ? ESP_NOW_ERROR_LED_ON_DURATION : ESP_NOW_ERROR_LED_OFF_DURATION)) {
                // Toggle LED state
                ledState = (blinkStep == 0) ? LOW : HIGH;  // ON for 500 ms, OFF for 50 ms
                digitalWrite(STATUS_LED_PIN, ledState);  // Apply the LED state

                // Move to the next blink step (0 = ON, 1 = OFF)
                blinkStep = (blinkStep + 1) % 2;  // Alternate between 0 and 1

                // Reset the blink start time
                blinkStartMillis = currentMillis;
            }
            break;
        
        case LED_FILE_SAVED:
            digitalWrite(STATUS_LED_PIN, HIGH);  // FILE_SAVED: Turn LED off
            for (int i = 0; i < FILE_SAVE_BLINK_COUNT; i++) {  // Send FILE_SAVE_BLINK_COUNT low/high pulses
                digitalWrite(STATUS_LED_PIN, LOW);
                delay(50); // ugh delay... it's not nice but it's a simple way to blink the LED, we'll fix this later ™
                digitalWrite(STATUS_LED_PIN, HIGH);
                delay(50);
            }
            setStatus(READY);  // Report the status
            break;
    }
}
