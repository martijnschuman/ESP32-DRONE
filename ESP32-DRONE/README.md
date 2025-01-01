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

## Usage
The main functionality of the project is implemented in the `src/main.cpp` file. Each sensor has its own setup and measurement functions.

## Sensor readings
To read the sensors I'm using a Circular Buffer approach

### What is a Circular Buffer?
A circular buffer is a fixed-size array that acts as a queue for storing a limited number of the most recent data points. When new data arrives:

The oldest data in the buffer is overwritten.
The buffer "wraps around" using a modulo operation (%).
This ensures that the memory usage remains constant, and we always calculate the average based on the latest N data points.

Example in Action
Let’s assume BUFFER_SIZE = 5 and we’re tracking accX. Here’s how the buffer evolves with each update:

### Example in Action
Let’s assume `BUFFER_SIZE = 5` and we’re tracking `accX`. Here’s how the buffer evolves with each update:

| Step | Buffer Contents        | `accXSum` | New Reading | `accXAverage` |
|------|-------------------------|-----------|-------------|---------------|
| 1    | `[0, 0, 0, 0, 0]`       | `0`       | `1.0`       | `0.2`         |
| 2    | `[1.0, 0, 0, 0, 0]`     | `1.0`     | `2.0`       | `0.6`         |
| 3    | `[1.0, 2.0, 0, 0, 0]`   | `3.0`     | `3.0`       | `1.2`         |
| 4    | `[1.0, 2.0, 3.0, 0, 0]` | `6.0`     | `4.0`       | `2.0`         |
| 5    | `[1.0, 2.0, 3.0, 4.0, 0]`| `10.0`   | `5.0`       | `3.0`         |
| 6    | `[6.0, 2.0, 3.0, 4.0, 5.0]`| `20.0` | `6.0`       | `4.0`         |

Notice how the buffer size remains fixed, and the average always considers the last 5 readings.