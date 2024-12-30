// command.h
#ifndef COMMAND_H
#define COMMAND_H

enum CommandStatusEnum {
    KEEP_ALIVE = 0x01,
    IS_ALIVE = 0x02,
    TAKE_PICTURE = 0x03,
    READY = 0x04,
    FILE_SAVED = 0x05,
    SD_INIT_ERROR = 0x06,
    SD_SAVE_ERROR = 0x07,
    CAMERA_INIT_ERROR = 0x08,
    CAMERA_TAKE_ERROR = 0x09,
    ESP_NOW_INIT_ERROR = 0x0A,
    ESP_NOW_ADD_PEER_ERROR = 0x0B,
    ESP_NOW_SEND_ERROR = 0x0C,
};

struct CommandPacket {
    CommandStatusEnum body;
};

extern CommandPacket command;  // Declare the external variable
extern CommandStatusEnum status;  // Declare the external variable

#endif // COMMAND_H
