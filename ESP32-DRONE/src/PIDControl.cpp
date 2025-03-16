#include "PIDControl.h"

// PID constants (tune these values)
float Kp = 1.0f, Ki = 0.02f, Kd = 0.5f;

// Variables for roll axis
float rollIntegral = 0.0f;
float prevRollError = 0.0f;
float rollOutput = 0.0f;

// Variables for pitch axis
float pitchIntegral = 0.0f;
float prevPitchError = 0.0f;
float pitchOutput = 0.0f;

// Variables for yaw axis
float yawIntegral = 0.0f;
float prevYawError = 0.0f;
float yawOutput = 0.0f;

// Helper function for PID computation for one axis
float computePID(float setpoint, float measurement, float dt, float &integral, float &prevError) {
    float error = setpoint - measurement;
    integral += error * dt;
    float derivative = (error - prevError) / dt;
    float output = Kp * error + Ki * integral + Kd * derivative;
    prevError = error;
    return output;
}

void setupPIDControl() {
    // Initialize integrals and previous errors
    rollIntegral = 0.0f;
    prevRollError = 0.0f;
    
    pitchIntegral = 0.0f;
    prevPitchError = 0.0f;
    
    yawIntegral = 0.0f;
    prevYawError = 0.0f;
}

void updatePIDControl() {
    // Compute the time delta (dt) in seconds
    // Assuming a fixed PID update interval (e.g., 10ms)
    float dt = 0.01f;  // 10 ms

    // Compute PID outputs for each axis
    rollOutput  = computePID(controlPacket.roll,  roll,  dt, rollIntegral,  prevRollError);
    pitchOutput = computePID(controlPacket.pitch, pitch, dt, pitchIntegral, prevPitchError);
    yawOutput   = computePID(controlPacket.yaw,   yaw,   dt, yawIntegral,   prevYawError);
    
    // Debug printing
    Serial.print("Roll Input: "); Serial.print(roll);
    Serial.print(", Roll Setpoint: "); Serial.println(controlPacket.roll);
    
    Serial.print("Pitch Input: "); Serial.print(pitch);
    Serial.print(", Pitch Setpoint: "); Serial.println(controlPacket.pitch);
    
    Serial.print("Yaw Input: "); Serial.print(yaw);
    Serial.print(", Yaw Setpoint: "); Serial.println(controlPacket.yaw);
    
    Serial.print("Roll Output: "); Serial.print(rollOutput);
    Serial.print(", Pitch Output: "); Serial.print(pitchOutput);
    Serial.print(", Yaw Output: "); Serial.println(yawOutput);
    
    // Mix and apply motor adjustments based on PID outputs
    applyMotorAdjustments(rollOutput, pitchOutput, yawOutput);
}
