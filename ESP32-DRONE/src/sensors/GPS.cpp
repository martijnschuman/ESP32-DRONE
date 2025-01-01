// src/gps.cpp
#include <GPS.h>

TinyGPSPlus gps;
HardwareSerial GPSModule(2);

float gpsLat, gpsLng, gpsAlt;
float gpsSpeed;
int gpsSatellites;
String gpsTime, gpsDate;

void setupGPS() {
    GPSModule.begin(9600, SERIAL_8N1, GPS_RXD2, GPS_TXD2); // GPS module at 9600 baud, RX2 = GPIO16, TX2 = GPIO17

    if (!GPSModule) {
        Serial.println("Failed to initialize GPS Serial communication!");
        throwError(GPS_ERROR);
        while (true);
    }

    Serial.println("GPS Module Initialized");
}

String formatTime(TinyGPSTime& time) {
    if (!time.isUpdated()) return "";
    return String(time.hour()) + ":" + String(time.minute()) + ":" + String(time.second()) + ":" + String(time.centisecond());
}

String formatDate(TinyGPSDate& date) {
    if (!date.isUpdated()) return "";
    return String(date.month()) + "/" + String(date.day()) + "/" + String(date.year());
}

String formatLocation(double value, bool valid) {
    return valid ? String(value, 6) : ""; // Format to 6 decimal places if valid
}

double formatValue(double value, bool valid) {
    return valid ? double(value) : 0; // Return the value as 0 if valid
}

int formatSatellites(int value, bool valid) {
    return valid ? value : 0; // Return the satellite count if valid
}

String getGPSTime() {
    if (gps.time.isValid()) {
        return String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
    }
    return "";
}

String getGPSDate() {
    if (gps.date.isValid()) {
        return String(gps.date.month()) + "/" + String(gps.date.day()) + "/" + String(gps.date.year());
    }
    return "";
}
String getGPSLocationLat() {
    return formatLocation(gps.location.lat(), gps.location.isValid());
}

String getGPSLocationLng() {
    return formatLocation(gps.location.lng(), gps.location.isValid());
}

int getGPSSpeed() {
    return formatValue(gps.speed.kmph(), gps.speed.isValid());
}

int getGPSSatellites() {
    return formatSatellites(gps.satellites.value(), gps.satellites.isValid());
}

float  getGPSAltitude() {
    return formatValue(gps.altitude.meters(), gps.altitude.isValid());
}

int getGPSHDOP() {
    return formatValue(gps.hdop.hdop(), gps.hdop.isValid());
}

void updateGPS() {
    while (GPSModule.available() > 0) {
        gps.encode(GPSModule.read());

        if (gps.location.isValid()) {
            gpsLat = gps.location.lat();
            gpsLng = gps.location.lng();
        } else {
            gpsLat = 0.0f;
            gpsLng = 0.0f;
        }

        gpsAlt = gps.altitude.isValid() ? gps.altitude.meters() : 0.0f;
        gpsSpeed = gps.speed.isValid() ? gps.speed.kmph() : 0.0f;
        gpsSatellites = gps.satellites.isValid() ? gps.satellites.value() : 0;
        gpsTime = gps.time.isValid() ? getGPSTime() : "";
        gpsDate = gps.date.isValid() ? getGPSDate() : "";
    }
}