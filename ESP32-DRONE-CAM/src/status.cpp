// status.cpp
#include "Status.h"

StatusEnum status;                          // Set initial status
unsigned long currentMillis = 0;            // Stores the current time
unsigned long previousMillis = 0;           // Stores the last time the LED was updated

unsigned long blinkStartMillis = 0;         // Time when the current blink started
bool ledState = LOW;                        // State of the LED (LOW = OFF, HIGH = ON)
int blinkStep = 0;                          // Step for blink pattern (0 for on, 1 for off)


// Function to report status
void reportStatus(StatusEnum newStatus) {
    switch (newStatus) {
        case SYSTEM_READY:
            showLEDStatus(SYSTEM_READY);
            break;
        case SYSTEM_ERROR:
            showLEDStatus(SYSTEM_ERROR);
            break;
        case SYSTEM_FILE_SAVED:
            showLEDStatus(SYSTEM_FILE_SAVED);
            break;
    }
    sendStatusToMaster(newStatus);  // Send status to master
}

// Function to control the LED based on the status
void showLEDStatus(StatusEnum ledStatus) {
    currentMillis = millis();  // Get the current time

    switch (ledStatus) {
        case SYSTEM_READY:
            // The LED is always ON for the "Ready" state
            digitalWrite(statusLEDPin, LOW);  // Inverted logic (LOW is ON)
            break;

        case SYSTEM_ERROR:
            // Error: blink pattern (500 ms ON, 50 ms OFF, repeat)
            if (currentMillis - blinkStartMillis >= (blinkStep == 0 ? errorOnDuration : errorOffDuration)) {
                // Toggle LED state
                ledState = (blinkStep == 0) ? LOW : HIGH;  // ON for 500 ms, OFF for 50 ms
                digitalWrite(statusLEDPin, ledState);  // Apply the LED state

                // Move to the next blink step (0 = ON, 1 = OFF)
                blinkStep = (blinkStep + 1) % 2;  // Alternate between 0 and 1

                // Reset the blink start time
                blinkStartMillis = currentMillis;
            }
            break;
        
        case SYSTEM_FILE_SAVED:
            digitalWrite(statusLEDPin, HIGH);  // FILE_SAVED: Turn LED off
            for (int i = 0; i < fileSaveBlinkCount; i++) {  // Send fileSaveBlinkCount low/high pulses
                digitalWrite(statusLEDPin, LOW);
                delay(50); // ugh delay... it's not nice but it's a simple way to blink the LED, we'll fix this later ™
                digitalWrite(statusLEDPin, HIGH);
                delay(50);
            }
            status = SYSTEM_READY;  // Reset status to READY after file saved
            break;
    }
}

// Function to send status to the master via GPIO pin
void sendStatusToMaster(StatusEnum currentStatus) {
    switch (currentStatus) {
        case SYSTEM_READY:
            digitalWrite(reportPin, HIGH);  // READY: Send HIGH to GPIO 2
            break;

        case SYSTEM_ERROR:
            digitalWrite(reportPin, LOW);  // ERROR: Send LOW to GPIO 2
            break;

        case SYSTEM_FILE_SAVED:
            // FILE_SAVED: Pulse pattern to indicate file saved
            for (int i = 0; i < fileSaveBlinkCount; i++) {  // Send 4 low/high pulses
                digitalWrite(reportPin, LOW);
                delay(50);
                digitalWrite(reportPin, HIGH);
                delay(50);
            }
            status = SYSTEM_READY;  // Reset status to READY after file saved
            break;
    }
}