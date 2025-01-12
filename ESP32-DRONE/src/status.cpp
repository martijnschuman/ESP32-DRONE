// status.cpp
#include "status.h"

int dec_digits[15] = {
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
    0b10000000, // 10: Decimal Point
    0b01110111, // A: Segments A, B, C, E, F, G
    0b01111100, // B: Segments C, D, E, F, G
    0b00111001, // C: Segments A, D, E, F
    0b01011110  // D: Segments B, C, D, E, G
};

// Function to setup the status LED
void setupStatusDisplay() {
    pinMode(SHIFT_REGISTER_DS_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_SHCP_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_STCP_PIN, OUTPUT);
}

// Function to set the status
void throwError(StatusEnum error) {
    setStatus(error);
    while (1) {
        reportStatus();
    }
}

void setStatus(StatusEnum newStatus) {
    if (droneState.status != newStatus) {
        droneState.status = newStatus;
        Serial.print("Status changed to: ");
        Serial.println(newStatus);
    }
}

void setFlightMode(FlightMode newMode) {
    if (droneState.flightMode != newMode) {
        droneState.flightMode = newMode;
        Serial.print("Flight mode changed to: ");
        Serial.println(newMode);
    }
}

void reportStatus() {
    if (droneState.status == READY) {
        displayDecimalPoint();
    } else {
        displayNumber(droneState.status);
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

StatusEnum getStatus() {
    return droneState.status;
}

FlightMode getFlightMode() {
    return droneState.flightMode;
}
