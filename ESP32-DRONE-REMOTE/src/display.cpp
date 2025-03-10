#include "display.h"

Adafruit_SSD1305 display(X_PIXELS, Y_PIXELS, &Wire, -1);


void setupDisplay() {
    if (!display.begin(SSD1305_I2C_ADDRESS, true)) {
        handleSetupError(DISPLAY_ERROR, "Error initializing display.");
    }

    display.setTextSize(1);
    display.setTextColor(WHITE);
    
    display.display();
    delay(1000);

    display.clearDisplay();
    Serial.println("Display setup complete.");
}

void displayTitel(String title) {
    display.setCursor(0, 0);
    display.println(title);
    display.drawRect(0, 9, 128, 1, WHITE);
}

void displayStartCalibration() {
    display.clearDisplay();

    displayTitel("Joystick Calibration");

    display.setCursor(0, 11);

    display.println("No joystick calibration found.");
    display.println("Press both joystick buttons to start calibration.");

    display.display();
}

void displayCalibration(int stage, const char* joystickName) {
    display.clearDisplay();

    displayTitel("Joystick Calibration");

    display.setCursor(0, 11);
    if (stage == 0) {
        display.println(String("Move the ") + joystickName + " joystick to the bottom-left corner and hold.");
    }
    else if (stage == 1) {
        display.println(String("Move the ") + joystickName + " joystick the top-right corner and hold.");
    }
    else if (stage == 2) {
        display.println(String("Release the ") + joystickName + " joystick to it's center position.");
    }
    else if (stage == 3) {
        display.println(String(joystickName) + " calibration complete!");
    }

    display.display();
}

void displayStartDroneConnection() {
    display.clearDisplay();

    displayTitel("Drone Connection");

    display.setCursor(0, 11);

    display.println("Attempting to connect to drone.");

    display.display();
}

void displayStartCameraConnection() {
    display.clearDisplay();

    displayTitel("Camera Connection");

    display.setCursor(0, 11);

    display.println("Attempting to connect to camera.");

    display.display();
}

void displayReadyToFly() {
    display.clearDisplay();

    displayTitel("Ready to Fly");

    display.setCursor(0, 11);

    display.println("Joysticks calibrated.");
    display.println("Press OK to start manual flight.");

    display.display();
}

void displayArmingDrone() {
    display.clearDisplay();

    displayTitel("Arming Drone");

    display.setCursor(0, 11);

    display.println("Arming drone for flight.");

    display.display();
}

void displayTelemetry() {
    display.clearDisplay();

    // Display height
    display.setCursor(0, 0);
    display.print("Hgt ");
    display.print(telemetryPacket.height);

    // Display picture count
    const uint8_t cameraIcon[8] = {
        0b00111100,
        0b01000010,  
        0b10111101,
        0b10100101, 
        0b10111101,
        0b10000001, 
        0b01000010,  
        0b00111100 
    };

    display.drawBitmap(106, 0, cameraIcon, 8, 8, 1);
    display.setCursor(116, 0);
    display.print(takenPictureCount);
    display.setCursor(90, 0);

    // Display left joystick values
    display.setCursor(0, 9);
    display.print("TY ");
    display.print(controlPacket.throttle);
    display.print(", ");
    display.print(controlPacket.yaw);

    // Display right joystick values
    display.setCursor(0, 18);
    display.print("RP ");
    display.print(controlPacket.roll);
    display.print(", ");
    display.print(controlPacket.pitch);

    // Display Gyro data
    display.setCursor(0, 27);
    display.print("GYR ");
    display.print(telemetryPacket.gyroX, 2);
    display.print(",");
    display.print(telemetryPacket.gyroY, 2);
    display.print(",");
    display.print(telemetryPacket.gyroZ, 2);

    // Display ACC data
    display.setCursor(0, 36);
    display.print("ACC ");
    display.print(telemetryPacket.accX, 2);
    display.print(",");
    display.print(telemetryPacket.accY, 2);
    display.print(",");
    display.print(telemetryPacket.accZ, 2);

    // Display battery info
    display.setCursor(0, 45);
    display.print("Bus ");
    display.print(telemetryPacket.busVoltage, 2);
    display.print(" Shu");
    display.print(telemetryPacket.shuntVoltage, 2);
    display.print("mV");

    display.setCursor(0, 54);
    display.print("Cur ");
    display.print(telemetryPacket.current, 2);
    display.print("mA");

    display.display();
}
