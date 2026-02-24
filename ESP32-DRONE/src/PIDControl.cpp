#include "PIDControl.h"

struct PID {
    float kp;
    float ki;
    float kd;
    float integral;
    float prevError;
};

PID angleRoll  = {1.5f, 0.0f, 0.0f, 0,0};
PID anglePitch = {1.5f, 0.0f, 0.0f, 0,0};

// 0.8 kp to much

PID rateRoll  = {0.6f, 0.0f, 0.0f, 0, 0};
PID ratePitch = {0.6f, 0.0f, 0.0f, 0, 0};
PID rateYaw   = {0.6f, 0.0f, 0.0f, 0, 0};


static float rollRateF = 0, pitchRateF = 0, yawRateF = 0;

float runPID(PID &pid, float setpoint, float measurement, float dt) {
    float error = setpoint - measurement;

    pid.integral += error * dt;
    pid.integral = constrain(pid.integral, -100.0f, 100.0f);

    float derivative = (error - pid.prevError) / dt;
    pid.prevError = error;

    return pid.kp*error + pid.ki*pid.integral + pid.kd*derivative;
}

static inline float clampf(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

static inline float normStick(float v) {
    // controlPacket.* expected in [-100 .. +100]
    return clampf(v / 100.0f, -1.0f, 1.0f);
}

void updatePIDControl(float dt = 0.005f) {
    constexpr float alpha = 0.2f;         // gyro LPF
    constexpr float maxTiltDeg = 5.0f;    // stick -> desired angle
    constexpr float maxYawRate = 5.0f;    // stick -> desired yaw rate (deg/s)
    constexpr float maxRateCmd = 5.0f;    // outer loop output clamp (deg/s)
    constexpr float maxAdjPct  = 5.0f;    // motor adjustment clamp (%)

    // 1) Normalize stick inputs to [-1..+1]
    const float rollIn  = normStick(controlPacket.roll);
    const float pitchIn = normStick(controlPacket.pitch);
    const float yawIn   = normStick(controlPacket.yaw);

    // 2) Convert sticks to physical setpoints
    const float desiredRollAngle  = rollIn  * maxTiltDeg;  // degrees
    const float desiredPitchAngle = pitchIn * maxTiltDeg;  // degrees
    const float desiredYawRate    = yawIn   * maxYawRate;  // deg/s

    // 3) Filter gyro rates (used by rate loop)
    rollRateF  += alpha * (rollRate  - rollRateF);
    pitchRateF += alpha * (pitchRate - pitchRateF);
    yawRateF   += alpha * (yawRate   - yawRateF);

    // 4) Outer loop: angle -> desired rate
    // IMPORTANT: measurement must be ANGLE (roll/pitch), not rate.
    float desiredRollRate  = runPID(angleRoll,  desiredRollAngle,  roll,  dt);
    float desiredPitchRate = runPID(anglePitch, desiredPitchAngle, -pitch, dt);

    desiredRollRate  = clampf(desiredRollRate,  -maxRateCmd, maxRateCmd);
    desiredPitchRate = clampf(desiredPitchRate, -maxRateCmd, maxRateCmd);

    // 5) Inner loop: rate -> motor adjustment
    float rollAdj  = runPID(rateRoll,  desiredRollRate,  rollRateF,  dt);
    float pitchAdj = runPID(ratePitch, desiredPitchRate, pitchRateF, dt);
    float yawAdj   = runPID(rateYaw,   desiredYawRate,   yawRateF,   dt);

    rollAdj  = clampf(rollAdj,  -maxAdjPct, maxAdjPct);
    pitchAdj = clampf(pitchAdj, -maxAdjPct, maxAdjPct);
    yawAdj   = clampf(yawAdj,   -maxAdjPct, maxAdjPct);

    applyMotorAdjustments(pitchAdj, rollAdj, yawAdj);
}