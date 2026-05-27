#include "control/algorithms/pid.h"

static float s_kp = 5.0f;
static float s_ki = 0.5f;
static float s_kd = 0.2f;
static float s_integral = 0.0f;
static float s_last_error = 0.0f;

void pid_init(float kp, float ki, float kd) {
    s_kp = kp;
    s_ki = ki;
    s_kd = kd;
    s_integral = 0.0f;
    s_last_error = 0.0f;
}

float pid_calculate(float temperature, float setpoint, float dt) {
    float error = setpoint - temperature;

    if (temperature > setpoint) {
        s_integral = 0.0f;
        return 0.0f;
    }

    float p = s_kp * error;
    s_integral += error * dt;
    if (s_integral > 1000.0f) s_integral = 1000.0f;
    if (s_integral < -1000.0f) s_integral = -1000.0f;
    float i = s_ki * s_integral;

    float d = 0.0f;
    if (dt > 0.0f) d = s_kd * (error - s_last_error) / dt;
    s_last_error = error;

    float out = p + i + d;
    if (out > 100.0f) out = 100.0f;
    if (out < 0.0f) out = 0.0f;
    return out;
}

void pid_reset() {
    s_integral = 0.0f;
    s_last_error = 0.0f;
}