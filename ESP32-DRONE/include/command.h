// command.h
#ifndef COMMAND_H
#define COMMAND_H

enum StatusEnum {
    READY = 0x00,
    I2C_MULTIPLEXER_ERROR = 0x01,
    IMU_ERROR = 0x02,
    LIDAR_ERROR = 0x03,
    GPS_ERROR = 0x04,
    ESP_NOW_INIT_ERROR = 0x05,
    ESP_NOW_ADD_PEER_ERROR = 0x06,
    ESP_NOW_SEND_ERROR = 0x07,
};

enum IsAliveEnum {
    IS_ALIVE_ACK = 0x01,
    IS_ALIVE_NACK = 0x02,
};

struct CommandPacket {
    StatusEnum body;
};

#endif // COMMAND_H
