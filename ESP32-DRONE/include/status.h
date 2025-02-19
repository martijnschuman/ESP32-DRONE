// include/status.h
#ifndef STATUS_H
#define STATUS_H

#include "config.h"

void setupStatusDisplay();
void setStatus(StatusEnum newStatus);
void setFlightMode(FlightMode newMode);
void throwError(StatusEnum error);
void reportStatus();
void displayStatusOnLED(int number);
void displayDecimalPoint();
void displayClear();

StatusEnum getStatus();
FlightMode getFlightMode();

#endif