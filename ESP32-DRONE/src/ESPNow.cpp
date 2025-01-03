#include "ESPNow.h"

// Slave MAC address
uint8_t remoteMAC[] = REMOTE_MAC_ADDRESS;

FirstConnectionRequestPacket firstConnection;

// Set up ESP-NOW
void setupESPNow() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        setStatus(ESP_NOW_INIT_ERROR);
        return;
    }

    // Set up callbacks
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceived);

    addPeer(remoteMAC);
}

void addPeer(uint8_t *peerMAC) {
    esp_now_peer_info_t slaveInfo;
    memset(&slaveInfo, 0, sizeof(slaveInfo));
    memcpy(slaveInfo.peer_addr, peerMAC, 6);
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
    if (dataLen == sizeof(ControlPacket)) {
        control = *reinterpret_cast<const ControlPacket*>(data);
        Serial.print("Joystick X: "); Serial.println(control.joystickX);
        Serial.print("Joystick Y: "); Serial.println(control.joystickY);
        Serial.print("Throttle: "); Serial.println(control.throttle);
        Serial.print("Yaw: "); Serial.println(control.yaw);
    } 
    else if (dataLen == sizeof(FirstConnectionRequestPacket)) {
        firstConnection = *reinterpret_cast<const FirstConnectionRequestPacket*>(data);
        Serial.println("First connection packet received.");
        if (firstConnection.status == READY) {
            Serial.println("Remote is ready.");
            sendDroneReady();
        }
    }
    else {
        Serial.println("Unknown packet received.");
    }
}

void sendDroneReady() {
    FirstConnectionRequestPacket packet;
    packet.status = READY;

    esp_err_t result = esp_now_send(remoteMAC, reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    if (result == ESP_OK) {
        Serial.println("Drone ready sent.");
    } else {
        Serial.println("Error sending drone ready.");
        throwError(ESP_NOW_SEND_ERROR);
    }
}
