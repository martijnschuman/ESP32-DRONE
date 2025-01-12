// include/status.h
#ifndef STATUS_H
#define STATUS_H

#include "config.h"

void setupStatusLEDs();
void displayLEDOKStatus();
void displayLEDErrorStatus();
void displayLEDBlankStatus();

void setStatus(StatusEnum newStatus);
void setFlightMode(FlightMode newMode);

StatusEnum getStatus();
FlightMode getFlightMode();

#endif