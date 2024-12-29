#include "ESPNow.h"

// Slave MAC address
uint8_t cameraMAC[] = CAMERA_MAC_ADDRESS;

// Variables
unsigned long lastKeepAlive = 0;
bool keepAliveAckReceived = false;

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
    esp_now_peer_info_t slaveInfo;
    memset(&slaveInfo, 0, sizeof(slaveInfo));
    memcpy(slaveInfo.peer_addr, cameraMAC, 6);
    slaveInfo.channel = 0;
    slaveInfo.encrypt = false;

    if (esp_now_add_peer(&slaveInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

// Callback when data is sent
void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("Keep-alive packet sent successfully.");
    } else {
        Serial.println("Error sending keep-alive packet.");
    }
}

// Callback when data is received
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    if (dataLen == sizeof(StatusEnum)) {
        StatusEnum receivedStatus = *reinterpret_cast<const StatusEnum*>(data);
        Serial.print("Status received from slave: ");
        Serial.println(receivedStatus); // Print or process the status
    }
    // Existing keep-alive handling
    else if (dataLen == 1 && data[0] == 1) {
        keepAliveAckReceived = true;
        Serial.println("Keep-alive acknowledgment received from slave.");
    }
}

void sendKeepAlive() {
    unsigned long currentMillis = millis();

    // Send keep-alive packet every 5 seconds
    if (currentMillis - lastKeepAlive >= CAMERA_KEEP_ALIVE_INTERVAL) {
        lastKeepAlive = currentMillis;
        keepAliveAckReceived = false;

        // Prepare keep-alive packet
        CommandPacket keepAlivePacket;
        keepAlivePacket.command = KEEP_ALIVE;

        // Send keep-alive packet
        esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&keepAlivePacket, sizeof(keepAlivePacket));

        if (result == ESP_OK) {
            Serial.println("Keep-alive packet sent, waiting for acknowledgment...");
        } else {
            Serial.println("Error sending keep-alive packet.");
        }
    }
}

void sendTakePictureCommand() {
    CommandPacket command;
    command.command = TAKE_PICTURE;

    esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&command, sizeof(command));
    if (result == ESP_OK) {
        Serial.println("Take picture command sent.");
    } else {
        Serial.println("Error sending take picture command.");
    }
}
