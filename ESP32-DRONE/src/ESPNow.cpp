#include "ESPNow.h"

// Slave MAC address
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
        Serial.println("Message sent successfully.");
    } else {
        Serial.println("Error sending packet.");
    }
}

// Callback when data is received
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    if (dataLen == sizeof(CommandPacket)) {
        CommandPacket receivedPacket = *reinterpret_cast<const CommandPacket*>(data);
        Serial.print("Packet received from slave: ");
        // Serial.println(receivedPacket.body); // Print or process the packet body

        // Handle specific packet types
        switch (receivedPacket.body) {
            case IS_ALIVE:
                isAliveAckReceived = true;
                Serial.println("Is-alive acknowledgment");
                break;
            case READY:
                Serial.println("Ready status");
                break;
            case FILE_SAVED:
                Serial.println("File saved status");
                break;
            case SD_INIT_ERROR:
                Serial.println("SD initialization error");
                break;
            case SD_SAVE_ERROR:
                Serial.println("SD save error");
                break;
            case CAMERA_INIT_ERROR:
                Serial.println("Camera initialization error");
                break;
            case CAMERA_TAKE_ERROR:
                Serial.println("Camera take error");
                break;
            case ESP_NOW_INIT_ERROR:
                Serial.println("ESP-NOW initialization error");
                break;
            case ESP_NOW_ADD_PEER_ERROR:
                Serial.println("ESP-NOW add peer error");
                break;
            case ESP_NOW_SEND_ERROR:
                Serial.println("ESP-NOW send error");
                break;
            default:
                Serial.println("Unknown status");
                Serial.println(receivedPacket.body);
                break;
        }
    } else {
        Serial.println("Received unknown packet.");
        for (int i = 0; i < dataLen; i++) {
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}

void sendIsCamAlive() {
    unsigned long currentMillis = millis();

    // Send keep-alive packet every 5 seconds
    if (currentMillis - lastKeepAlive >= CAMERA_KEEP_ALIVE_INTERVAL) {
        lastKeepAlive = currentMillis;
        isAliveAckReceived = false;

        // Prepare keep-alive packet
        CommandPacket keepAlivePacket;
        keepAlivePacket.body = KEEP_ALIVE;

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
    command.body = TAKE_PICTURE;

    esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&command, sizeof(command));
    if (result == ESP_OK) {
        Serial.println("Take picture command sent.");
    } else {
        Serial.println("Error sending take picture command.");
    }
}
