// include/status.h
#ifndef STATUS_H
#define STATUS_H

#include "config.h"

void setupStatusDisplay();
void setStatus(StatusEnum newStatus);
void setFlightMode(FlightMode newMode);
void throwError(StatusEnum error);
void reportStatus();
void displayNumber(int number);
void displayDecimalPoint();
void displayClear();

#endif