#include "ESPNow.h"

uint8_t remoteMAC[] = REMOTE_MAC_ADDRESS;

// Set up ESP-NOW
void setupESPNow() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        throwError(ESP_NOW_INIT_ERROR);
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
         Serial.print("Message sent to: ");
        for (int i = 0; i < 6; i++) {
            Serial.print(macAddr[i], HEX);
            if (i < 5) {
                Serial.print(":");
            }
        }
        Serial.println();
    } else {
        Serial.println("Error sending packet.");
        throwError(ESP_NOW_SEND_ERROR);
    }
}

// Callback when data is received
void onDataReceived(const uint8_t *macAddr, const uint8_t *data, int dataLen) {
    if (data == nullptr || dataLen <= 0) {
        Serial.println("Received invalid data.");
        return;
    }

    if (dataLen == sizeof(ControlPacket)) {
        controlPacket = *reinterpret_cast<const ControlPacket*>(data);
    } 
    else if (dataLen == sizeof(DroneStatePacket)) {
        droneStatePacket = *reinterpret_cast<const DroneStatePacket*>(data);
        Serial.print("Drone status packet received: ");

        if (droneStatePacket.droneState.status == START_CONNECTION) {
            sendDroneReady();
        }
        else if (droneStatePacket.droneState.status == READY && droneStatePacket.droneState.flightMode == MANUAL) {
            setStatus(READY);
            setFlightMode(MANUAL);
            isConnectedToRemote = true;
        }
    }
    else {
        Serial.println("Unknown packet received.");
        Serial.print("Data length: ");
        Serial.println(dataLen);
    }
}

void sendDroneReady() {
    DroneStatePacket packet;
    packet.droneState.status = READY;
    packet.droneState.flightMode = GROUND;

    esp_err_t result = esp_now_send(remoteMAC, (uint8_t*)&packet, sizeof(packet));
    if (result == ESP_OK) {
        Serial.println("Drone ready sent.");
        setStatus(READY);
        setFlightMode(GROUND);
    }
}

void sendTelemetry() {
    TelemetryPacket telemetryPacket;

    telemetryPacket.status = droneState.status;
    telemetryPacket.accX = accX;
    telemetryPacket.accY = accY;
    telemetryPacket.accZ = accZ;
    telemetryPacket.gyroX = gyroX;
    telemetryPacket.gyroY = gyroY;
    telemetryPacket.gyroZ = gyroZ;
    telemetryPacket.height = lidarHeight;
    telemetryPacket.gpsLat = gpsLat;
    telemetryPacket.gpsLng = gpsLng;
    telemetryPacket.gpsAlt = gpsAlt;
    telemetryPacket.gpsSpeed = gpsSpeed;
    telemetryPacket.gpsSatellites = gpsSatellites;
    telemetryPacket.gpsTime = gpsTime;
    telemetryPacket.gpsDate = gpsDate;

    esp_err_t result = esp_now_send(remoteMAC, (uint8_t*)&telemetryPacket, sizeof(telemetryPacket));
    if (result == ESP_OK) {
        Serial.println("Telemetry sent.");
    }
}