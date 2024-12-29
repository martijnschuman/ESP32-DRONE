// include/status.h
#ifndef STATUS_H
#define STATUS_H

#include "Config.h"

enum StatusEnum {
    SYSTEM_READY,
    SYSTEM_FILE_SAVED,
    SYSTEM_ERROR
};

extern StatusEnum status;

void reportStatus(StatusEnum newStatus);
void showLEDStatus(StatusEnum ledStatus);
void sendStatusToMaster(StatusEnum status);

#endif
