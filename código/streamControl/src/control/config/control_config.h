// ============================================================================
// CONTROL CONFIG - Parámetros del controlador
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

#include <stdint.h>

// ============================================================================
// PID CONFIGURATION
// ============================================================================

// PID gains (float para precisión)
#define PID_KP_DEFAULT   1.0f
#define PID_KI_DEFAULT   0.1f
#define PID_KD_DEFAULT   0.05f

// PID output limits
#define PID_OUTPUT_MIN   0.0f    // 0%
#define PID_OUTPUT_MAX   100.0f   // 100%

// PID integral limits (anti-windup)
#define PID_INTEGRAL_MAX  1000.0f
#define PID_INTEGRAL_MIN -1000.0f

// PID sample time (ms)
#define PID_SAMPLE_TIME_MS  100

// ============================================================================
// CONTROL MODES
// ============================================================================

typedef enum {
    CONTROL_MODE_OFF = 0,
    CONTROL_MODE_MANUAL,
    CONTROL_MODE_PID,
    CONTROL_MODE_RST,
    CONTROL_MODE_IDENT
} control_mode_t;

// ============================================================================
// MANUAL CONFIG
// ============================================================================

#define MANUAL_POWER_DEFAULT  0     // Potencia manual por defecto (%)
#define MANUAL_POWER_MIN   0
#define MANUAL_POWER_MAX 100

#endif // CONTROL_CONFIG_H