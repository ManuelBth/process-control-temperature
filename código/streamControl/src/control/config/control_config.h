// ============================================================================
// CONTROL CONFIG - PID and RST control parameters
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

#include <stdint.h>

// ============================================================================
// PID PARAMETERS
// ============================================================================

// Default PID gains (tunable via serial)
#define DEFAULT_KP  10.0f
#define DEFAULT_KI  0.5f
#define DEFAULT_KD  2.0f

// PID limits
#define PID_INTEGRAL_MAX   10000.0f  // Max integral term
#define PID_INTEGRAL_MIN   -10000.0f // Min integral term
#define PID_OUTPUT_MAX     100        // Max output (%)
#define PID_OUTPUT_MIN     0          // Min output (%)

// ============================================================================
// RST CONTROL PARAMETERS
// ============================================================================

// RST polynomial coefficients (default)
#define RST_R0  1.0f
#define RST_R1  -0.5f
#define RST_R2  0.1f

#define RST_S0  2.0f
#define RST_S1  -1.5f
#define RST_S2  0.3f

#define RST_T0  1.0f
#define RST_T1  0.0f
#define RST_T2  0.0f

// ============================================================================
// CONTROL TIMING
// ============================================================================

// Control loop sample rate (ms) - 100ms = 10Hz
#define CONTROL_SAMPLE_RATE_MS    100
#define CONTROL_SAMPLE_RATE_HZ   10

// ============================================================================
// CONTROL LIMITS
// ============================================================================

// Temperature limits
#define CONTROL_TEMP_MIN          1800    // 18°C minimum setpoint (×100)
#define CONTROL_TEMP_MAX          12000   // 120°C maximum setpoint (×100)
#define CONTROL_TEMP_HYSTERESIS   50      // 0.5°C hysteresis (×100)

// Safety limits
#define CONTROL_OVERTEMP_THRESHOLD  12000  // 120°C (×100)
#define CONTROL_OVERTEMP_HYSTERESIS 1000   // 10°C hysteresis

// Output limits
#define CONTROL_OUTPUT_MIN  0   // 0%
#define CONTROL_OUTPUT_MAX  100 // 100%

// ============================================================================
// AUTO-TUNE PARAMETERS
// ============================================================================

#define AUTOTUNE_Y_UPPER   70    // Upper output limit %
#define AUTOTUNE_Y_LOWER   30    // Lower output limit %
#define AUTOTUNE_SAMPLES   250  // Number of samples

#endif // CONTROL_CONFIG_H
