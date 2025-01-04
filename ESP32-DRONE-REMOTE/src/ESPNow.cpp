#include "ESPNow.h"

uint8_t droneMAC[] = REMOTE_MAC_ADDRESS;
FirstConnectionRequestPacket firstConnection = {};

void setupESPNow() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        setStatus(ESP_NOW_INIT_ERROR);
        return;
    }

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceived);

    addPeer(droneMAC);
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

void onDataSent(const uint8_t *macAddr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.print("Message sent to: ");
        for (int i = 0; i < 6; i++) {
            Serial.print(macAddr[i], HEX);
            if (i < 5) {
                Serial.print(":");
            }
        }
    } else {
        Serial.print("Error sending packet: ");
        Serial.println(status);
    }
}

void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    if (data == nullptr || dataLen <= 0) {
        Serial.println("Received invalid data.");
        return;
    }

    if (dataLen == sizeof(TelemetryPacket)) {
        telemetry = *reinterpret_cast<const TelemetryPacket *>(data);
    } else if (dataLen == sizeof(FirstConnectionRequestPacket)) {
        firstConnection = *reinterpret_cast<const FirstConnectionRequestPacket *>(data);
        if (firstConnection.status == READY) {
            Serial.println("Drone is ready.");
            isConnectedToDrone = true;
            setStatus(READY);
        }
    } else {
        Serial.println("Unknown packet received.");
        Serial.print("Data length: ");
        Serial.println(dataLen);
    }
}

void connectToDrone() {
    FirstConnectionRequestPacket command;
    command.status = START_CONNECTION;

    esp_err_t result = esp_now_send(droneMAC, (uint8_t*)&command, sizeof(command));
    if (result == ESP_OK) {
        Serial.println("Connection request sent.");
    } else {
        Serial.println("Error sending connection request.");
    }
}
