// telemetry.h

#include "telemetry.h"

TelemetryPacket telemetry;
ControlPacket control;

void printTelemetry(){
    Serial.print("Status: "); Serial.println(telemetry.status);
    Serial.print("Acc X: "); Serial.println(telemetry.accX);
    Serial.print("Acc Y: "); Serial.println(telemetry.accY);
    Serial.print("Acc Z: "); Serial.println(telemetry.accZ);
    Serial.print("Gyro X: "); Serial.println(telemetry.gyroX);
    Serial.print("Gyro Y: "); Serial.println(telemetry.gyroY);
    Serial.print("Gyro Z: "); Serial.println(telemetry.gyroZ);
    Serial.print("Temp: "); Serial.println(telemetry.temp);
    Serial.print("Height: "); Serial.println(telemetry.height);
    Serial.print("GPS Lat: "); Serial.println(telemetry.gpsLat);
    Serial.print("GPS Lng: "); Serial.println(telemetry.gpsLng);
    Serial.print("GPS Alt: "); Serial.println(telemetry.gpsAlt);
    Serial.print("GPS Speed: "); Serial.println(telemetry.gpsSpeed);
    Serial.print("GPS Satellites: "); Serial.println(telemetry.gpsSatellites);
    Serial.print("GPS Time: "); Serial.println(telemetry.gpsTime);
    Serial.print("GPS Date: "); Serial.println(telemetry.gpsDate);
}

void sendControl() {
    control.joystickX = getLeftJoystickX();
    control.joystickY = getLeftJoystickY();
    control.throttle = getLeftJoystickY();
    control.yaw = getLeftJoystickX();

    esp_err_t result = esp_now_send(droneMAC, reinterpret_cast<uint8_t*>(&control), sizeof(control));
    if (result == ESP_OK) {
        Serial.println("Telemetry sent.");
    } else {
        Serial.println("Error sending telemetry.");
        displayErrorStatus();
    }
}
