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

StatusEnum getStatus() {
    return droneState.status;
}

FlightMode getFlightMode() {
    return droneState.flightMode;
}
