// PIDControl.cpp
#include "PIDControl.h"

// PID structure for each axis
struct PID {
    float kp;
    float ki;
    float kd;
    float previousError;
    float integral;
};

// Initialize PID controllers for pitch, roll, and yaw (tune these gains as needed)
PID pidRoll  = { 0.11, 0.01, 0.00, 0.0, 0.0 };
PID pidPitch = { 0.11, 0.01, 0.00, 0.0, 0.0 };
PID pidYaw   = { 0.1, 0.00, 0.00, 0.0, 0.0 };

// Compute PID output given a setpoint and measurement.
float computePID(PID &pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;

    // Deadband: ignore small errors to reduce jitter
    const float deadband = 1.0;  // adjust as needed
    if (fabs(error) < deadband) {
        error = 0;
    }

    pid.integral += error * dt;
    float derivative = (error - pid.previousError) / dt;
    float output = pid.kp * error + pid.ki * pid.integral + pid.kd * derivative;
    pid.previousError = error;
    return output;
}

// Updated PID update function with low-pass filtering on outputs.
void updatePIDControl() {
    // Convert the PID update interval from milliseconds to seconds.
    float dt = PID_UPDATE_INTERVAL / 1000.0f;

    // Scale desired setpoints using your stick sensitivity.
    float desiredPitch = STICK_SENSITIVITY * controlPacket.pitch;
    float desiredRoll  = STICK_SENSITIVITY * controlPacket.roll;
    float desiredYaw   = STICK_SENSITIVITY * controlPacket.yaw;

    // Compute raw PID corrections for each axis.
    float newPitchCorrection = computePID(pidPitch, desiredPitch, pitch, dt);
    float newRollCorrection  = computePID(pidRoll,  desiredRoll, roll, dt);
    float newYawCorrection   = computePID(pidYaw,   desiredYaw, yaw, dt);

    // Apply a low-pass filter to smooth out the corrections.
    // Adjust filterAlpha (0.0-1.0) for desired responsiveness.
    static float filteredPitchCorrection = 0;
    static float filteredRollCorrection  = 0;
    static float filteredYawCorrection   = 0;
    const float filterAlpha = 0.8f;  // Higher alpha -> more smoothing

    filteredPitchCorrection = filterAlpha * filteredPitchCorrection + (1 - filterAlpha) * newPitchCorrection;
    filteredRollCorrection  = filterAlpha * filteredRollCorrection  + (1 - filterAlpha) * newRollCorrection;
    filteredYawCorrection   = filterAlpha * filteredYawCorrection   + (1 - filterAlpha) * newYawCorrection;

    // Use the filtered outputs for motor mixing.
    applyMotorAdjustments(filteredPitchCorrection, filteredRollCorrection, filteredYawCorrection);
}


void processPIDCommand(const String& cmd) {
    if (cmd.startsWith("SHOW")) {
        Serial.printf("Pitch: P=%.3f I=%.3f D=%.3f\n", pidPitch.kp, pidPitch.ki, pidPitch.kd);
        Serial.printf("Roll : P=%.3f I=%.3f D=%.3f\n", pidRoll.kp, pidRoll.ki, pidRoll.kd);
        Serial.printf("Yaw  : P=%.3f I=%.3f D=%.3f\n", pidYaw.kp, pidYaw.ki, pidYaw.kd);
        return;
    }

    char type, axis;
    float value;
    if (sscanf(cmd.c_str(), "%c %c %f", &type, &axis, &value) == 3) {
        PID* pid = nullptr;
        if (axis == 'P') pid = &pidPitch;
        else if (axis == 'R') pid = &pidRoll;
        else if (axis == 'Y') pid = &pidYaw;

        if (pid) {
            if (type == 'P') pid->kp = value;
            else if (type == 'I') pid->ki = value;
            else if (type == 'D') pid->kd = value;

            Serial.printf("Updated %c for %c to %.3f\n", type, axis, value);
        }
    } else {
        Serial.println("Invalid command. Format: P R 0.15");
    }
}