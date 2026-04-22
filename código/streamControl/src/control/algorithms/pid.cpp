// ============================================================================
// PID ALGORITHM
// ============================================================================

#include "../data/control_data.h"
#include "../config/control_config.h"

// ============================================================================
// PID CALCULATE
// ============================================================================

float pid_calculate(float temperature, float setpoint, float dt) {
    // Calcular error
    float error = setpoint - temperature;

    // Término proporcional
    float p_term = g_control_state.kp * error;

    // Término integral (con anti-windup)
    g_control_state.integral += error * dt;

    // Clamp integral
    if (g_control_state.integral > PID_INTEGRAL_MAX) {
        g_control_state.integral = PID_INTEGRAL_MAX;
    } else if (g_control_state.integral < PID_INTEGRAL_MIN) {
        g_control_state.integral = PID_INTEGRAL_MIN;
    }

    float i_term = g_control_state.ki * g_control_state.integral;

    // Término derivativo
    float d_term = 0.0f;
    if (dt > 0.0f) {
        d_term = g_control_state.kd * (error - g_control_state.last_error) / dt;
    }

    // Guardar último error
    g_control_state.last_error = error;

    // Calcular salida total
    float output = p_term + i_term + d_term;

    // Clamp salida a límites
    if (output > PID_OUTPUT_MAX) {
        output = PID_OUTPUT_MAX;
    } else if (output < PID_OUTPUT_MIN) {
        output = PID_OUTPUT_MIN;
    }

    return output;
}

// ============================================================================
// PID RESET
// ============================================================================

void pid_reset(void) {
    g_control_state.integral = 0.0f;
    g_control_state.last_error = 0.0f;
}