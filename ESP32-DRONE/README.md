# ESP32 Drone Project

This project is designed to run on an ESP32 development board and interfaces with various sensors including LIDAR, GPS, IMU, and more. The project is built using PlatformIO.

## Project Structure
- **include/**: Contains header files for the project.
- **lib/**: Contains project-specific libraries.
- **src/**: Contains the main source code and sensor implementations.
- **test/**: Contains unit tests for the project.
- **.vscode/**: Contains Visual Studio Code settings and configurations.
- **.pio/**: Contains PlatformIO build files and dependencies.
- **platformio.ini**: PlatformIO project configuration file.
- **README.md**: This file.

## Dependencies

This project uses the following libraries:
- Adafruit Unified Sensor
- TinyGPSPlus
- DFRobot LIDAR07
- Adafruit BusIO
- Wire

## Setup and Installation

1. Install [PlatformIO](https://platformio.org/) in your preferred IDE (e.g., VSCode).
2. Clone this repository.
3. Open the project folder in your IDE.
4. Build and upload the project to your ESP32 board using PlatformIO.

