// telemetry.h

#include "telemetry.h"

int getThrottleValue() {
    if (getRightJoystickY() > 0) {
        controlPacket.throttle = getRightJoystickY();
    } else {
        controlPacket.throttle = 0;
    }
}

void sendControl() {
    controlPacket.joystickX = getLeftJoystickX();
    controlPacket.joystickY = getLeftJoystickY();
    controlPacket.throttle = getThrottleValue();
    controlPacket.yaw = getRightJoystickX();

    esp_err_t result = esp_now_send(droneMAC, reinterpret_cast<uint8_t*>(&controlPacket), sizeof(controlPacket));
    if (result == ESP_OK) {
        Serial.println("Telemetry sent.");
    } else {
        Serial.println("Error sending telemetry.");
        displayLEDErrorStatus();
    }
}
