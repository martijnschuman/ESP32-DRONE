# ESP32 Drone Project
This repository contains the code for a drone built using ESP32 microcontrollers. The project is developed using PlatformIO and includes multiple sensors such as LIDAR, GPS, and IMU. The project is divided into three main components: the drone, the camera, and the remote controller.

## Project Structure
```
ESP32-DRONE/
    .gitignore
    .pio/
    .vscode/
    include/
    lib/
    src/
    test/
ESP32-DRONE-CAM/
    .gitignore
    .pio/
    .vscode/
    include/
    lib/
    src/
    test/
ESP32-DRONE-REMOTE/
    .gitignore
    .pio/
    .vscode/
    include/
    lib/
    src/
    test/
README.md
```

### ESP32-DRONE
This directory contains the main drone code, which interfaces with various sensors and communicates with the remote controller.

#### Main Loop
The main loop in src/main.cpp performs the following tasks:

Report Status: Periodically reports the drone's status.
Update Sensors: Updates the IMU and LIDAR sensor readings at specified intervals.
Display Telemetry: Prints telemetry data to the serial monitor.
Transmit Telemetry: Sends telemetry data to the remote controller via ESP-NOW.

### ESP32-DRONE-CAM
This directory contains the code for the camera module, which captures images and saves them to an SD card.

#### Main Loop
The main loop in src/main.cpp performs the following tasks:

Report Status: Periodically reports the camera's status.
Handle Commands: Listens for commands from the main drone to take pictures and save them to the SD card.

### ESP32-DRONE-REMOTE
This directory contains the code for the remote controller, which communicates with the drone and sends control commands.

#### Main Loop
The main loop in src/main.cpp performs the following tasks:

Calibrate Joysticks: Calibrates the joysticks if not already calibrated.
Connect to Drone: Attempts to connect to the drone and establish communication.
Send Control Commands: Sends control commands to the drone based on joystick inputs.


## Dependencies
This project uses the following libraries:

Adafruit Unified Sensor
TinyGPSPlus
DFRobot LIDAR07
Adafruit BusIO
Wire
LiquidCrystal_I2C
Setup and Installation
Install PlatformIO in your preferred IDE (e.g., VSCode).
Clone this repository.
Open the project folder in your IDE.
Build and upload the project to your ESP32 boards using PlatformIO.
Usage
The main functionality of the project is implemented in the main.cpp files of each component. Each sensor and module has its own setup and measurement functions.

## Sensor Readings
To read the sensors, a Circular Buffer approach is used to maintain a fixed-size array that acts as a queue for storing a limited number of the most recent data points.

```plantuml
@startuml
participant Remote as ESP32_REMOTE
participant Drone as ESP32_DRONE

ESP32_REMOTE -> ESP32_REMOTE: loop()
ESP32_REMOTE -> ESP32_REMOTE: checkStatus()
alt Status is CALIBRATING
    ESP32_REMOTE -> ESP32_REMOTE: displayStartCalibration()
    ESP32_REMOTE -> ESP32_REMOTE: startCalibrateJoysticks()
    alt Joysticks Calibrated
        ESP32_REMOTE -> ESP32_REMOTE: setStatus(START_CONNECTION)
    end
else Status is START_CONNECTION
    ESP32_REMOTE -> ESP32_REMOTE: connectToDrone()
    ESP32_REMOTE -> ESP32_DRONE: sendConnectionRequest()
end

ESP32_DRONE -> ESP32_DRONE: loop()
ESP32_DRONE -> ESP32_DRONE: checkStatus()
alt Status is START_CONNECTION
    ESP32_DRONE -> ESP32_DRONE: setStatus(READY)
    ESP32_DRONE -> ESP32_DRONE: setFlightMode(GROUND)
    ESP32_DRONE -> ESP32_REMOTE: sendDroneReady()
end

ESP32_REMOTE -> ESP32_REMOTE: onDataReceived()
ESP32_REMOTE -> ESP32_REMOTE: setStatus(READY)
ESP32_REMOTE -> ESP32_REMOTE: setFlightMode(GROUND)
ESP32_REMOTE -> ESP32_REMOTE: displayReadyToFly()

ESP32_REMOTE -> ESP32_REMOTE: loop()
ESP32_REMOTE -> ESP32_REMOTE: checkStatus()
alt Status is READY and FlightMode is GROUND
    ESP32_REMOTE -> ESP32_REMOTE: checkOKButton()
    alt OK Button Pressed
        ESP32_REMOTE -> ESP32_DRONE: sendFlightModeToDrone(MANUAL)
    end
end

ESP32_DRONE -> ESP32_DRONE: onDataReceived()
ESP32_DRONE -> ESP32_DRONE: setFlightMode(MANUAL)

ESP32_REMOTE -> ESP32_REMOTE: loop()
ESP32_REMOTE -> ESP32_REMOTE: checkStatus()
alt Status is READY and FlightMode is MANUAL
    ESP32_REMOTE -> ESP32_REMOTE: sendControl()
    ESP32_REMOTE -> ESP32_DRONE: sendControlPacket()
end

ESP32_DRONE -> ESP32_DRONE: onDataReceived()
ESP32_DRONE -> ESP32_DRONE: updateControl()
@enduml
```
