#include "ESPNow.h"

// Slave MAC address
uint8_t remoteMAC[] = REMOTE_MAC_ADDRESS;

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
        setStatus(ESP_NOW_INIT_ERROR);
        return;
    }

    // Set up callbacks
    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceived);

    // Register peer
    addPeer(remoteMAC);
}

void addPeer(uint8_t *peerMAC) {
    // Register peer
    esp_now_peer_info_t slaveInfo;
    memset(&slaveInfo, 0, sizeof(slaveInfo));
    memcpy(slaveInfo.peer_addr, peerMAC, 6);
    slaveInfo.channel = 0;
    slaveInfo.encrypt = false;

    if (esp_now_add_peer(&slaveInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        setStatus(ESP_NOW_ADD_PEER_ERROR);
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
    } else {
        Serial.println("Unknown packet received.");
    }
}

bool testRemoteConnection() {
    // Send test packet to drone to check connection
    ControlPacket testPacket;
    testPacket.joystickX = 0;
    testPacket.joystickY = 0;
    testPacket.throttle = 0;
    testPacket.yaw = 0;

    esp_err_t result = esp_now_send(remoteMAC, reinterpret_cast<uint8_t*>(&testPacket), sizeof(testPacket));
    if (result == ESP_OK) {
        Serial.println("Test packet sent.");
        setStatus(READY);
        return true;
    } else {
        Serial.println("Error sending test packet. Retrying in " + String(CONNECTION_TEST_INTERVAL) + "ms.");
        setStatus(ESP_NOW_SEND_ERROR);
        return false;
    }
}
