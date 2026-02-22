#include "PIDControl.h"

struct PID {
    float kp;
    float ki;
    float kd;
    float integral;
    float prevError;
};

PID angleRoll  = {4.0f, 0.0f, 0.0f, 0,0};
PID anglePitch = {2.0f, 0.0f, 0.0f, 0,0};

PID rateRoll  = {0.06f, 0.0f, 0.002f, 0,0};
PID ratePitch = {0.04f, 0.0f, 0.001f, 0, 0};
PID rateYaw   = {0.15f, 0.0f, 0.000f, 0,0};

float runPID(PID &pid, float setpoint, float measurement, float dt) {
    float error = setpoint - measurement;

    pid.integral += error * dt;
    pid.integral = constrain(pid.integral, -100.0f, 100.0f);

    float derivative = (error - pid.prevError) / dt;
    pid.prevError = error;

    return pid.kp*error + pid.ki*pid.integral + pid.kd*derivative;
}

void updatePIDControl() {
    float dt = 0.005f;

    if(dt <= 0 || dt > 0.02f) return;

    float desiredRollAngle  = controlPacket.roll  * 30.0f;
    float desiredPitchAngle = controlPacket.pitch * 15.0f;
    float desiredYawRate    = controlPacket.yaw   * 150.0f;

    // --- OUTER LOOP (Angle → desired rate)
    float desiredRollRate  = runPID(angleRoll,  desiredRollAngle,  roll,  dt);
    float desiredPitchRate = runPID(anglePitch, desiredPitchAngle, -pitch, dt);

    desiredRollRate = constrain(desiredRollRate, -200.0f, 200.0f);
    desiredPitchRate = constrain(desiredPitchRate, -200.0f, 200.0f);

    // --- INNER LOOP (Rate PID)
    float rollOutput  = runPID(rateRoll,  desiredRollRate,  rollRate,  dt);
    float pitchOutput = runPID(ratePitch, desiredPitchRate, pitchRate, dt);
    float yawOutput   = runPID(rateYaw,   desiredYawRate,   yawRate,   dt);

    rollOutput  = constrain(rollOutput,  -25.0f, 25.0f);
    pitchOutput = constrain(pitchOutput, -25.0f, 25.0f);
    yawOutput   = constrain(yawOutput,   -25.0f, 25.0f);

    applyMotorAdjustments(pitchOutput, rollOutput, yawOutput);
}