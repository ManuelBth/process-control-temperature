// ============================================================================
// CONTROL DATA - Control domain state and data structures
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef CONTROL_DATA_H
#define CONTROL_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "central_data.h"

// ============================================================================
// CONTROL STATE STRUCTURES
// ============================================================================

// Control loop state
typedef struct {
    // Mode
    control_mode_t mode;
    control_mode_t previous_mode;

    // Setpoint (×100)
    temperature_raw_t setpoint;

    // Output (0-100%)
    percent_t output;

    // PID internal state
    float pid_integral;
    temperature_raw_t pid_prev_error;
    bool pid_first_run;

    // RST internal state
    float r_coeffs[3];
    float s_coeffs[3];
    float t_coeffs[3];
    temperature_raw_t error_history[3];
    percent_t output_history[3];

    // Auto-tune state
    bool auto_tune_active;
    bool auto_tune_complete;
} control_state_t;

// ============================================================================
// GLOBAL STATE (external)
// ============================================================================

extern control_state_t g_control_state;

// ============================================================================
// ACCESS MACROS
// ============================================================================

#define g_ctrl_mode         g_control_state.mode
#define g_ctrl_setpoint    g_control_state.setpoint
#define g_ctrl_output    g_control_state.output
#define g_ctrl_integral  g_control_state.pid_integral
#define g_ctrl_prev_error g_control_state.pid_prev_error

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void control_data_init(void);

#endif // CONTROL_DATA_H