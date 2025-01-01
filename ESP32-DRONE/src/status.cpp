// status.cpp
#include "status.h"

// Set initial status
StatusEnum status = READY;

int dec_digits[11] = {
    0b00111111, // 0: Segments A, B, C, D, E, F
    0b00000110, // 1: Segments B, C
    0b01011011, // 2: Segments A, B, D, E, G
    0b01001111, // 3: Segments A, B, C, D, G
    0b01100110, // 4: Segments B, C, F, G
    0b01101101, // 5: Segments A, C, D, F, G
    0b01111101, // 6: Segments A, C, D, E, F, G
    0b00000111, // 7: Segments A, B, C
    0b01111111, // 8: Segments A, B, C, D, E, F, G
    0b01101111, // 9: Segments A, B, C, D, F, G
    0b10000000  // 10: Decimal Point
};

// Function to setup the status LED
void setupStatusDisplay() {
    pinMode(SHIFT_REGISTER_DS_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_SHCP_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_STCP_PIN, OUTPUT);
}

// Function to set the status
void setStatus(StatusEnum newStatus) {
    status = newStatus;
}

void throwError(StatusEnum error) {
    setStatus(error);
    while (1) {
        reportStatus();
    }
}

void reportStatus() {
    if (status == READY) {
        displayDecimalPoint();
    } else {
        displayNumber(status);
    }
}

void displayNumber(int number) {
    digitalWrite(SHIFT_REGISTER_STCP_PIN, LOW);
    shiftOut(SHIFT_REGISTER_DS_PIN, SHIFT_REGISTER_SHCP_PIN, MSBFIRST, dec_digits[number]);
    digitalWrite(SHIFT_REGISTER_STCP_PIN, HIGH);
}

void displayDecimalPoint() {
    digitalWrite(SHIFT_REGISTER_STCP_PIN, LOW);
    shiftOut(SHIFT_REGISTER_DS_PIN, SHIFT_REGISTER_SHCP_PIN, MSBFIRST, dec_digits[10]); // DP pattern
    digitalWrite(SHIFT_REGISTER_STCP_PIN, HIGH);
}

void displayClear() {
    digitalWrite(SHIFT_REGISTER_STCP_PIN, LOW);
    shiftOut(SHIFT_REGISTER_DS_PIN, SHIFT_REGISTER_SHCP_PIN, MSBFIRST, 0);
    digitalWrite(SHIFT_REGISTER_STCP_PIN, HIGH);
}
