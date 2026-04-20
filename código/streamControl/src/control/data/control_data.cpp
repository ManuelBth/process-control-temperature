// ============================================================================
// CONTROL DATA IMPLEMENTATION
// ============================================================================

#include "control_data.h"

// ============================================================================
// GLOBAL STATE
// ============================================================================

control_state_t g_control_state = {
    .mode = CONTROL_MODE_OFF,
    .enabled = false,
    .kp = PID_KP_DEFAULT,
    .ki = PID_KI_DEFAULT,
    .kd = PID_KD_DEFAULT,
    .setpoint = 0.0f,
    .integral = 0.0f,
    .last_error = 0.0f,
    .output = 0.0f,
    .manual_power = MANUAL_POWER_DEFAULT,
    .temperature = 0.0f
};

// ============================================================================
// FUNCTIONS
// ============================================================================

void control_data_init(void) {
    g_control_state.mode = CONTROL_MODE_OFF;
    g_control_state.enabled = false;
    g_control_state.kp = PID_KP_DEFAULT;
    g_control_state.ki = PID_KI_DEFAULT;
    g_control_state.kd = PID_KD_DEFAULT;
    g_control_state.setpoint = 0.0f;
    g_control_state.integral = 0.0f;
    g_control_state.last_error = 0.0f;
    g_control_state.output = 0.0f;
    g_control_state.manual_power = MANUAL_POWER_DEFAULT;
    g_control_state.temperature = 0.0f;
}