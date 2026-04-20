// ============================================================================
// ACTUATION CONFIG - Actuator configuration
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef ACTUATION_CONFIG_H
#define ACTUATION_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// Use types from central_data if available, otherwise define locally
#ifdef CENTRAL_DATA_H
#include "central_data.h"
#else
typedef uint8_t percent_t;
#endif

// ============================================================================
// TRIAC CONFIGURATION
// ============================================================================

// TRIAC pin (defined in pins.h)
// #define TRIAC_PIN ...

// TRIAC timing
#define TRIAC_ZERO_CROSS_DELAY_US    50    // Zero-cross delay (µs)
#define TRIAC_FIRE_PULSE_WIDTH_US  50   // Fire pulse width (µs)

// TRIAC power levels
#define TRIAC_POWER_MIN    0     // Min power (%)
#define TRIAC_POWER_MAX   100    // Max power (%)

// TRIAC firing angle (0-90° corresponds to 0-5ms half-cycle)
// Lower angle = higher power
#define TRIAC_ANGLE_MIN   10    // Min firing angle (°)
#define TRIAC_ANGLE_MAX  140   // Max firing angle (°)

// TRIAC timing table (angle in degrees → delay in µs)
#define TRIAC_DELAY_TABLE_SIZE 91

// ============================================================================
// FAN 1 (PWM) CONFIGURATION
// ============================================================================

// FAN1 pin (defined in pins.h)
// #define FAN1_PIN ...

// FAN1 PWM configuration
#define FAN1_PWM_FREQ      25000   // PWM frequency (Hz)
#define FAN1_PWM_RESOLUTION 8        // PWM resolution (bits)
#define FAN1_PWM_CHANNEL   0        // LEDC channel

// FAN1 speed limits
#define FAN1_SPEED_MIN   0      // Min speed (%)
#define FAN1_SPEED_MAX   100     // Max speed (%)

// ============================================================================
// FAN 2 (ON/OFF) CONFIGURATION
// ============================================================================

// FAN2 pin (defined in pins.h)
// #define FAN2_PIN ...

// ============================================================================
// RELAY CONFIGURATION
// ============================================================================

// Relay pin (defined in pins.h)
// #define RELAY_PIN ...

// ============================================================================
// ACTUATION TIMING
// ============================================================================

// Actuation update interval (ms)
#define ACTUATION_UPDATE_INTERVAL_MS  100

// ============================================================================
// SAFETY CONFIGURATION
// ============================================================================

// Thermal limit before fan forced ON
#define ACTUATION_THERMAL_LIMIT  11000 // 110°C (×100)

// Fan hysteresis (°C × 100)
#define ACTUATION_FAN_HYSTERESIS 500   // 5°C

// Emergency shutdown delay (ms)
#define ACTUATION_EMERGENCY_DELAY_MS  50

#endif // ACTUATION_CONFIG_H