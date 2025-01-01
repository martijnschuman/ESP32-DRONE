// ESPNOW.cpp

#include "ESPNow.h"

// Slave MAC address
uint8_t droneMAC[] = REMOTE_MAC_ADDRESS;
uint8_t cameraMAC[] = CAMERA_MAC_ADDRESS;

// Variables
unsigned long lastKeepAlive = 0;
bool isAliveAckReceived = false;

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

// void sendIsCamAlive() {
//     unsigned long currentMillis = millis();

//     // Send keep-alive packet every 5 seconds
//     if (currentMillis - lastKeepAlive >= CAMERA_KEEP_ALIVE_INTERVAL) {
//         lastKeepAlive = currentMillis;
//         isAliveAckReceived = false;

//         // Prepare keep-alive packet
//         CommandPacket keepAlivePacket;
//         keepAlivePacket.body = KEEP_ALIVE;

//         // Send keep-alive packet
//         esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&keepAlivePacket, sizeof(keepAlivePacket));

//         if (result == ESP_OK) {
//             Serial.println("Keep-alive packet sent, waiting for acknowledgment...");
//         } else {
//             Serial.println("Error sending keep-alive packet.");
//         }
//     }
// }

void sendTakePictureCommand() {
    CommandPacket command;
    command.body = TAKE_PICTURE;

    esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&command, sizeof(command));
    if (result == ESP_OK) {
        Serial.println("Take picture command sent.");
    } else {
        Serial.println("Error sending take picture command.");
    }
}