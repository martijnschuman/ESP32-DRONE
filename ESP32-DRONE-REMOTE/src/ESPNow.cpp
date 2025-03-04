#include "ESPNow.h"

uint8_t droneMAC[] = DRONE_MAC_ADDRESS;
uint8_t cameraMAC[] = CAMERA_MAC_ADDRESS;

void setupESPNow() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        handleSetupError(ESP_NOW_INIT_ERROR, "Error initializing ESP-NOW");
    }

    // esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceived);

    addPeer(droneMAC);
    addPeer(cameraMAC);
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
        telemetryPacket = *reinterpret_cast<const TelemetryPacket *>(data);
        Serial.println("Telemetry packet received.");
    } else if (dataLen == sizeof(DroneStatePacket)) {
        droneStatePacket = *reinterpret_cast<const DroneStatePacket *>(data);
        Serial.println("Drone state packet received.");

        if (droneStatePacket.droneState.status == READY && droneStatePacket.droneState.flightMode == GROUND) {
            Serial.println("Drone connection is ready.");
            isConnectedToDrone = true;
            setStatus(READY);
            setFlightMode(GROUND);	
        }

        if (droneStatePacket.droneState.status == READY && droneStatePacket.droneState.flightMode == MANUAL) {
            Serial.println("Drone ready for flight.");
            setStatus(READY);
            setFlightMode(MANUAL);
        }

    } else if (dataLen == sizeof(CameraPacket)) {
        cameraPacket = *reinterpret_cast<const CameraPacket *>(data);
        Serial.println("Camera packet received.");
        Serial.println(cameraPacket.cameraMode);

        if (cameraPacket.cameraMode == CAM_READY) {
            Serial.println("Camera is ready.");
            isConnectedToCam = true;
        } else if (cameraPacket.cameraMode == CAM_PICTURE_SAVED) {
            Serial.println("Picture saved.");
            takenPictureCount++;
        }
    }
    else {
        Serial.println("Unknown packet received.");
        Serial.print("Data length: ");
        Serial.println(dataLen);
    }
}

void connectToDrone() {
    DroneStatePacket packet;
    packet.droneState.status = START_CONNECTION;
    packet.droneState.flightMode = BOOT;

    esp_err_t result = esp_now_send(droneMAC, (uint8_t*)&packet, sizeof(packet));
    if (result == ESP_OK) {
        Serial.println("Connection request sent.");
    }
}

void connectToCamera() {
    CameraPacket packet;
    packet.cameraMode = CAM_BOOT;

    esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&packet, sizeof(packet));
    if (result == ESP_OK) {
        Serial.println("Camera connection request sent.");
    }
}

void sendFlightModeToDrone(FlightMode mode) {
    DroneStatePacket packet;
    packet.droneState.status = READY;
    packet.droneState.flightMode = mode;

    esp_err_t result = esp_now_send(droneMAC, (uint8_t*)&packet, sizeof(packet));
    if (result == ESP_OK) {
        Serial.print("Mode change request sent, mode: ");
        Serial.println(mode);
        setFlightMode(mode);
    }
}

void sendCameraCommand(CameraMode mode) {
    CameraPacket packet;
    packet.cameraMode = mode;

    esp_err_t result = esp_now_send(cameraMAC, (uint8_t*)&packet, sizeof(packet));
    if (result == ESP_OK) {
        Serial.print("Camera mode change request sent, mode: ");
        Serial.println(mode);
    }
}