// command.h
#ifndef COMMAND_H
#define COMMAND_H

enum CommandStatusEnum {
    KEEP_ALIVE = 0x01,
    TAKE_PICTURE = 0x02,
    READY = 0x03,
    FILE_SAVED = 0x04,
    SD_INIT_ERROR = 0x05,
    SD_SAVE_ERROR = 0x06,
    CAMERA_INIT_ERROR = 0x07,
    CAMERA_TAKE_ERROR = 0x08,
    ESP_NOW_INIT_ERROR = 0x09,
    ESP_NOW_ADD_PEER_ERROR = 0x0A,
    ESP_NOW_SEND_ERROR = 0x0B,
};

struct CommandPacket {
    CommandStatusEnum body;
};

extern CommandPacket command;  // Declare the external variable
extern CommandStatusEnum status;  // Declare the external variable

#endif // COMMAND_H
