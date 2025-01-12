// telemetry.h

#include "telemetry.h"

int getThrottleValue() {
    int throttleValue = 0;

    if (getRightJoystickY() > 0) {
        throttleValue = getRightJoystickY();
    } else {
        throttleValue = 0;
    }

    return throttleValue;
}

void sendControl() {
    controlPacket.joystickX = getLeftJoystickX();
    controlPacket.joystickY = getLeftJoystickY();
    controlPacket.throttle = getThrottleValue();
    controlPacket.yaw = getRightJoystickX();

    esp_err_t result = esp_now_send(droneMAC, reinterpret_cast<uint8_t*>(&controlPacket), sizeof(controlPacket));
    if (result == ESP_OK) {
        Serial.println("Control package sent.");
    } else {
        Serial.println("Error sending control packet.");
        displayLEDErrorStatus();
    }
}
