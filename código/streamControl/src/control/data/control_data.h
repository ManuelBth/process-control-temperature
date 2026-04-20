// ============================================================================
// CONTROL DATA - Estado global del controlador
// ============================================================================

#ifndef CONTROL_DATA_H
#define CONTROL_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "control_config.h"

// ============================================================================
// CONTROL STATE
// ============================================================================

typedef struct {
    // Modo actual
    control_mode_t mode;

    // Habilitación
    volatile bool enabled;

    // PID parameters
    float kp;
    float ki;
    float kd;

    // Setpoint (temperatura objetivo)
    float setpoint;

    // PID state
    float integral;
    float last_error;

    // Salida calculada (0-100%)
    float output;

    // Manual power (cuando mode = MANUAL)
    float manual_power;

    // Temperatura actual (desde sensor)
    float temperature;
} control_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern control_state_t g_control_state;

// ============================================================================
// MACROS
// ============================================================================

#define g_ctrl_mode      g_control_state.mode
#define g_ctrl_en       g_control_state.enabled
#define g_ctrl_kp       g_control_state.kp
#define g_ctrl_ki       g_control_state.ki
#define g_ctrl_kd       g_control_state.kd
#define g_ctrl_sp       g_control_state.setpoint
#define g_ctrl_out     g_control_state.output
#define g_ctrl_temp    g_control_state.temperature

// ============================================================================
// FUNCTIONS
// ============================================================================

void control_data_init(void);

#endif // CONTROL_DATA_H