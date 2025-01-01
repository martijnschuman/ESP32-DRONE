// status.cpp

#include "status.h"

void setupStatusLEDs() {
    pinMode(STATUS_RED_LED, OUTPUT);
    pinMode(STATUS_GREEN_LED, OUTPUT);
}

void displayOKStatus() {
    digitalWrite(STATUS_RED_LED, LOW);
    digitalWrite(STATUS_GREEN_LED, HIGH);
}

void displayErrorStatus() {
    digitalWrite(STATUS_RED_LED, HIGH);
    digitalWrite(STATUS_GREEN_LED, LOW);
}

void displayBlankStatus() {
    digitalWrite(STATUS_RED_LED, LOW);
    digitalWrite(STATUS_GREEN_LED, LOW);
}
