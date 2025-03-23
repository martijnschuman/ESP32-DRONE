#include "IMU.h"
#include <Wire.h>

// Define the MPU interrupt pin (adjust for your board if needed)
#define INTERRUPT_PIN 34

// Create an MPU6050 instance using the DMP library
MPU6050 mpu;

// MPU control/status variables
bool dmpReady = false;          // set true if DMP init was successful
uint8_t mpuIntStatus;           // holds actual interrupt status from MPU
uint8_t devStatus;              // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;            // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64];         // FIFO storage buffer

// Orientation/motion variables from DMP
Quaternion q;                 // [w, x, y, z] quaternion container
VectorFloat gravity;          // [x, y, z] gravity vector
float ypr[3];                 // [yaw, pitch, roll] (radians)

// Global orientation angles (in degrees) for PID control
float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;

// Interrupt flag to indicate when DMP data is ready
volatile bool mpuInterrupt = false;

// Interrupt service routine – sets the flag when new data is ready
void dmpDataReady() {
    mpuInterrupt = true;
}

void setupIMU() {
    // Initialize I2C communication
    Wire.begin();
    Wire.setClock(400000); // Use 400kHz I2C clock for faster communication

    // Initialize the MPU6050 device
    mpu.initialize();
    if (!mpu.testConnection()) {
        Serial.println(F("MPU6050 connection failed"));
        throwError(IMU_ERROR);
        while (1) { delay(10); }
    }
    Serial.println(F("MPU6050 connection successful"));

    // Initialize DMP
    devStatus = mpu.dmpInitialize();
    if (devStatus == 0) {
        // Calibrate accelerometer and gyroscope offsets
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();

        // Enable the DMP
        mpu.setDMPEnabled(true);

        // Setup interrupt to capture DMP data ready events
        pinMode(INTERRUPT_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // Mark DMP as ready
        dmpReady = true;
        // Retrieve expected DMP packet size for later processing
        packetSize = mpu.dmpGetFIFOPacketSize();

        Serial.println(F("MPU6050 DMP initialized and ready!"));
    } else {
        // Handle DMP initialization error
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
        throwError(IMU_ERROR);
        while (1) { delay(10); }
    }
}

void updateIMU() {
    if (!dmpReady) return;

    // If an interrupt has been signaled or a packet is ready in FIFO,
    // then process the available data.
    if (mpuInterrupt || mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        mpuInterrupt = false;
        if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
            // Retrieve quaternion from FIFO packet
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            // Compute gravity vector based on current quaternion
            mpu.dmpGetGravity(&gravity, &q);
            // Compute yaw, pitch, and roll (in radians)
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            // Convert from radians to degrees
            float newYaw   = -ypr[0] * 180 / M_PI;
            float newPitch = ypr[1] * 180 / M_PI;
            float newRoll  = ypr[2] * 180 / M_PI;

            // Apply low pass filtering for pitch and yaw
            static float filteredPitch = newPitch;
            static float filteredYaw = newYaw;
            const float alpha = 0.15; // Adjust this coefficient as needed
            filteredPitch = alpha * newPitch + (1 - alpha) * filteredPitch;
            filteredYaw   = alpha * newYaw   + (1 - alpha) * filteredYaw;

            // Update global variables (using filtered values for pitch and yaw)
            pitch = filteredPitch;
            yaw   = filteredYaw;
            roll  = newRoll; // Roll remains unfiltered (modify if needed)
        }
    }
}
