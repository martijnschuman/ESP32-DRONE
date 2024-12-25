// src/gps.cpp
#include <GPS.h>
#include <Config.h>

TinyGPSPlus gps;
HardwareSerial GPSModule(2);

void setupGPS(){
    GPSModule.begin(9600, SERIAL_8N1, GPS_RXD2, GPS_TXD2); // GPS module at 9600 baud, RX2 = GPIO16, TX2 = GPIO17

    if (!GPSModule){
        Serial.println("Failed to initialize GPS Serial communication!");
        while (true); // Halt the program
    }

    Serial.println("GPS Module Initialized");
}

String getGPSTime(){
    String time = "";
    if (gps.time.isUpdated()){
        time += gps.time.hour();
        time += ":";
        time += gps.time.minute();
        time += ":";
        time += gps.time.second();
        time += ":";
        time += gps.time.centisecond();
    }
    
    return time;
}

String getGPSDate(){
    String date = "";
    if (gps.date.isUpdated()){
        date += gps.date.month();
        date += "/";
        date += gps.date.day();
        date += "/";
        date += gps.date.year();
    }
    
    return date;
}

String getGPSLocation(){
    String location = "";
    if (gps.location.isUpdated()){
        location += gps.location.lat();
        location += ", ";
        location += gps.location.lng();
    }
    
    return location;
}

String getGPSSpeed(){
    String speed = "";
    if (gps.speed.isUpdated()){
        speed += gps.speed.kmph();
    }
    
    return speed;
}

String getGPSSatellites(){
    String satellites = "";
    if (gps.satellites.isUpdated()){
        satellites += gps.satellites.value();
    }
    
    return satellites;
}

String getGPSAltitude(){
    String altitude = "";
    if (gps.altitude.isUpdated()){
        altitude += gps.altitude.meters();
    }
    
    return altitude;
}

String getGPSHDOP(){
    String hdop = "";
    if (gps.hdop.isUpdated()){
        hdop += gps.hdop.hdop();
    }
    
    return hdop;
}

void displayGPSData(){
    // Print out GPS data if available
    if (gps.location.isUpdated()){
        Serial.print(getGPSLocation());
    }

    if (gps.satellites.isUpdated()){
        Serial.print("Satellites: ");
        Serial.println(getGPSSatellites());
    }

    if (gps.altitude.isUpdated()){
        Serial.print("Altitude (m): ");
        Serial.println(gps.altitude.meters());
    }
    
    if (gps.speed.isUpdated()){
        Serial.print("Speed (km/h): ");
        Serial.println(getGPSSpeed());
    }

    if (gps.time.isUpdated()){
        Serial.print("Time: ");
        Serial.println(getGPSTime());
    }

    if (gps.date.isUpdated()){
        Serial.print("Date: ");
        Serial.println(getGPSDate());
    }

    if (gps.hdop.isUpdated()){
        Serial.print("HDOP: ");
        Serial.println(getGPSHDOP());
    }
}
