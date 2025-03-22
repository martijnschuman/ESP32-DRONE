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
PID pidPitch = { 1.0, 0.0, 0.1, 0.0, 0.0 };
PID pidRoll  = { 1.0, 0.0, 0.1, 0.0, 0.0 };
PID pidYaw   = { 1.0, 0.0, 0.1, 0.0, 0.0 };

// Compute PID output given a setpoint and measurement.
float computePID(PID &pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;
    pid.integral += error * dt;
    float derivative = (error - pid.previousError) / dt;
    float output = pid.kp * error + pid.ki * pid.integral + pid.kd * derivative;
    pid.previousError = error;
    return output;
}

// This function should be called every PID_UPDATE_INTERVAL (e.g., 50ms)
void updatePIDControl() {
    // Convert the interval from milliseconds to seconds.
    float dt = PID_UPDATE_INTERVAL / 1000.0;

    float desiredPitch = STICK_SENSITIVITY * controlPacket.pitch;
    float desiredRoll  = STICK_SENSITIVITY * controlPacket.roll;
    float desiredYaw   = STICK_SENSITIVITY * controlPacket.yaw;

    // Compute PID corrections for each axis
    float pitchCorrection = computePID(pidPitch, desiredPitch, pitch, dt);
    float rollCorrection  = computePID(pidRoll,  desiredRoll,  roll,  dt);
    float yawCorrection   = computePID(pidYaw,   desiredYaw,   yaw,   dt);

    // Mix the corrections into the motor outputs.
    // (The mixing function will add these to the base throttle and distribute them to the ESCs.)
    applyMotorAdjustments(pitchCorrection, rollCorrection, yawCorrection);
}
