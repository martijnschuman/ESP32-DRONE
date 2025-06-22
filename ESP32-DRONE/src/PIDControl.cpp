// PIDControl.cpp
#include "PIDControl.h"

// PID structure for each axis
// kp: Proportional gain - how strongly the controller reacts to the current error
// ki: Integral gain - how strongly the controller reacts to the accumulation of past errors
// kd: Derivative gain - how strongly the controller reacts to the rate of change of the error
// previousError: Stores the last error value for derivative calculation
// integral: Accumulates the error over time for the integral term
struct PID {
    float kp;
    float ki;
    float kd;
    float previousError;
    float integral;
};

// Initialize PID controllers for pitch, roll, and yaw (tune these gains as needed)
// pidRoll: Controls the roll axis (left/right tilt)
// pidPitch: Controls the pitch axis (forward/backward tilt)
// pidYaw: Controls the yaw axis (rotation around vertical axis)
PID pidRoll  = { 0.10, 0.0, 0.0, 0.0, 0.0 };
PID pidPitch = { 0.18, 0.0, 0.0, 0.0, 0.0 };
PID pidYaw   = { 0.10, 0.0, 0.0, 0.0, 0.0 };

// Compute PID output given a setpoint and measurement.
// setpoint: The desired value (target angle)
// measured: The current value (measured angle)
// dt: Time since last update (in seconds)
float computePID(PID &pid, float setpoint, float measured, float dt) {
    float error = setpoint - measured;

    // Deadband: ignore small errors to reduce jitter
    // If the error is within the deadband, treat it as zero
    const float deadband = 1.0;  // adjust as needed
    if (fabs(error) < deadband) {
        error = 0;
    }

    // Integral term: accumulate error over time
    pid.integral += error * dt;
    // Derivative term: rate of change of error
    float derivative = (error - pid.previousError) / dt;
    // PID output: weighted sum of P, I, D terms
    float output = pid.kp * error + pid.ki * pid.integral + pid.kd * derivative;
    // Store current error for next derivative calculation
    pid.previousError = error;
    return output;
}

// Updated PID update function with low-pass filtering on outputs.
// This function is called periodically to update the PID corrections for each axis.
void updatePIDControl()
{
    float dt = PID_UPDATE_INTERVAL / 1000.0f;

    // stick → desired angles (roll/pitch)  (unchanged)
    float desiredPitch = STICK_SENSITIVITY * controlPacket.pitch;
    float desiredRoll  = STICK_SENSITIVITY * controlPacket.roll;

    // --- roll / pitch corrections ------------------------------------------------
    float pitchCorr = computePID(pidPitch, desiredPitch, -pitch, dt);
    float rollCorr  = computePID(pidRoll , desiredRoll , roll , dt);

    // --- yaw-rate control  --------------------------------------------------------
    float desiredYawRate = controlPacket.yaw * MAX_YAW_RATE_DPS;   // °/s
    float yawCorr = computePID(pidYaw,
                               desiredYawRate,
                               currentYawRate,      // measured °/s from IMU.cpp
                               dt);

    // low-pass filter (optional – same code as before)
    static float fPitch=0, fRoll=0, fYaw=0;
    const float alpha = 0.80f;
    fPitch = alpha*fPitch + (1-alpha)*pitchCorr;
    fRoll  = alpha*fRoll  + (1-alpha)*rollCorr;
    fYaw   = alpha*fYaw   + (1-alpha)*yawCorr;

    applyMotorAdjustments(fPitch, fRoll, fYaw);
}
