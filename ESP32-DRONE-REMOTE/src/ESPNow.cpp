// ESPNOW.cpp

#include "ESPNow.h"

// Slave MAC address
uint8_t droneMAC[] = REMOTE_MAC_ADDRESS;
uint8_t cameraMAC[] = CAMERA_MAC_ADDRESS;

void setupESPNow() {
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Set up callbacks
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceived);

    // Register peer
    addPeer(droneMAC);
    addPeer(cameraMAC);
}

void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent." : "Error sending packet.");
}

void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    if (dataLen == sizeof(TelemetryPacket)) {
        telemetry = *reinterpret_cast<const TelemetryPacket*>(data);
    } else {
        Serial.println("Unknown packet received.");
    }
}


// void sendTakePictureCommand() {
//     CommandPacket command;
//     command.body = TAKE_PICTURE;

//     esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&command, sizeof(command));
//     if (result == ESP_OK) {
//         Serial.println("Take picture command sent.");
//     } else {
//         Serial.println("Error sending take picture command.");
//     }
// }