// ESPNow.cpp
#include "ESPNow.h"
#include "cam.h"

uint8_t remoteMAC[] = REMOTE_MAC_ADDRESS;

void setupESPNow() {
    // Initialize Wi-Fi in STA mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        setStatus(CAM_ESP_NOW_INIT_ERROR);
        return;
    }

    // Register callback for received data
    esp_now_register_recv_cb(onDataReceived);

    // Add master as a peer
    addPeer(remoteMAC);

    Serial.println("ESP-NOW setup complete.");
}

void addPeer(uint8_t *peerMAC) {
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Error adding peer.");
    }
}

// Callback when data is received
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    Serial.println("Data received.");

    if (dataLen == sizeof(CameraMode)) {
        CameraPacket command = *reinterpret_cast<const CameraPacket*>(data);
        switch (command.cameraMode) {
            case CAM_BOOT: {
                Serial.println("Boot command received.");

                // Send status back to master
                setStatus(CAM_READY);
                sendResponseToRemote();

                break;
            }
            case CAM_TAKE_PICTURE: {
                Serial.println("Take picture command received.");
                takePicture();

                // Send status back to master
                sendResponseToRemote();
                setStatus(CAM_READY);
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
void sendResponseToRemote() {
    CameraPacket packet;
    packet.cameraMode = status;  // Set the status in the CommandPacket

    Serial.println("Sending response to remote.");
    Serial.print("Status: ");
    Serial.println(packet.cameraMode);

    if (esp_now_send(remoteMAC, (uint8_t*)&packet, sizeof(packet)) != ESP_OK) {
        Serial.println("Error sending command to master.");
        setStatus(CAM_ESP_NOW_SEND_ERROR);
    }
}
