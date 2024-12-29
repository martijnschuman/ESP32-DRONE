// ESPNow.cpp
#include "ESPNow.h"
#include "cam.h"

uint8_t mainMAC[] = MAIN_MAC_ADDRESS;

void setupESPNow() {
    // Initialize Wi-Fi in STA mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        setStatus(ESP_NOW_INIT_ERROR);
        return;
    }

    // Register callback for received data
    esp_now_register_recv_cb(onDataReceived);

    // Add master as a peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, mainMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer.");
        setStatus(ESP_NOW_ADD_PEER_ERROR);
        return;
    }
}

// Callback when data is received
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    if (dataLen == sizeof(CommandPacket)) {
        CommandPacket command = *reinterpret_cast<const CommandPacket*>(data);
        switch (command.body) {
            case KEEP_ALIVE: {
                Serial.println("Keep-alive packet received from master.");
                
                // Respond with acknowledgment
                uint8_t ackMessage = 1;
                esp_now_send(macAddr, &ackMessage, sizeof(ackMessage));
                break;
            }
            case TAKE_PICTURE: {
                Serial.println("Take picture command received.");
                takePicture();

                // Send status back to master
                sendCommandToMaster();
                break;
            }
            default: {
                Serial.println("Unknown command received.");
                break;
            }
        }
    }
}

// Function to send a CommandPacket with status to the master
void sendCommandToMaster() {
    CommandPacket packet;
    packet.body = status;  // Set the status in the CommandPacket

    if (esp_now_send(mainMAC, (uint8_t*)&packet, sizeof(packet)) != ESP_OK) {
        Serial.println("Error sending command to master.");
        setStatus(ESP_NOW_SEND_ERROR);
    }
}
