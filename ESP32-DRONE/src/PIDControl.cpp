//PIDControl.cpp
#include "PIDControl.h"

// PID variables
float rollInput = 0.0f, rollSetpoint = 0.0f, rollOutput = 0.0f;
float pitchInput = 0.0f, pitchSetpoint = 0.0f, pitchOutput = 0.0f;
float yawInput = 0.0f, yawSetpoint = 0.0f, yawOutput = 0.0f;

// PID constants (tune these values)
float Kp = 1.0f, Ki = 0.02f, Kd = 0.5f;

// Create PID controllers
QuickPID rollPID(&rollInput, &rollSetpoint, &rollOutput, Kp, Ki, Kd, QuickPID::Action::direct);
QuickPID pitchPID(&pitchInput, &pitchSetpoint, &pitchOutput, Kp, Ki, Kd, QuickPID::Action::direct);
QuickPID yawPID(&yawInput, &yawSetpoint, &yawOutput, Kp, Ki, Kd, QuickPID::Action::direct);


void setupPIDControl() {
    rollPID.SetMode(QuickPID::Control::automatic);
    pitchPID.SetMode(QuickPID::Control::automatic);
    yawPID.SetMode(QuickPID::Control::automatic);
}

void updatePIDControl() {
    // Set current orientation as inputs
    rollInput = roll;
    pitchInput = pitch;
    yawInput = yaw;

    // Desired setpoints
    rollSetpoint = controlPacket.roll;
    pitchSetpoint = controlPacket.pitch;
    yawSetpoint = controlPacket.yaw;

    // Print inputs and setpoints to debug
    Serial.print("Roll Input: ");
    Serial.print(rollInput);
    Serial.print(", Roll Setpoint: ");
    Serial.println(rollSetpoint);
    Serial.print("Pitch Input: ");
    Serial.print(pitchInput);
    Serial.print(", Pitch Setpoint: ");
    Serial.println(pitchSetpoint);
    Serial.print("Yaw Input: ");
    Serial.print(yawInput);
    Serial.print(", Yaw Setpoint: ");
    Serial.println(yawSetpoint);

    // Compute PID outputs
    rollPID.Compute();
    pitchPID.Compute();
    yawPID.Compute();

    // Print the PID outputs for debugging
    Serial.print("Roll Output: ");
    Serial.print(rollOutput);
    Serial.print(", Pitch Output: ");
    Serial.print(pitchOutput);
    Serial.print(", Yaw Output: ");
    Serial.println(yawOutput);

    // Adjust motor speeds based on PID outputs
    applyMotorAdjustments(rollOutput, pitchOutput, yawOutput);
}
