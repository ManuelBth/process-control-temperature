// ============================================================================
// ACTUATION DATA - Actuation domain state and data structures
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef ACTUATION_DATA_H
#define ACTUATION_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "actuation_config.h"

// ============================================================================
// ACTUATION STATE STRUCTURES
// ============================================================================

// Actuator states
typedef struct {
    // TRIAC (heater)
    percent_t triac_power;
    uint8_t triac_firing_angle;
    bool triac_enabled;
    bool triac_active;

    // FAN1 (PWM)
    percent_t fan1_speed;
    bool fan1_enabled;
    bool fan1_active;

    // FAN2 (ON/OFF)
    bool fan2_on;
    bool fan2_enabled;

    // Main relay
    bool relay_on;
    bool relay_enabled;

    // Safety
    bool safety_override;
    bool emergency_stop;

    // Statistics
    uint32_t triac_fire_count;
    uint32_t fan1_runtime_ms;
    uint32_t fan2_runtime_ms;
} actuation_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern actuation_state_t g_actuation_state;

// ============================================================================
// ACCESS MACROS
// ============================================================================

#define g_triac_pwr       g_actuation_state.triac_power
#define g_triac_angle     g_actuation_state.triac_firing_angle
#define g_triac_en        g_actuation_state.triac_enabled
#define g_fan1_spd        g_actuation_state.fan1_speed
#define g_fan1_en         g_actuation_state.fan1_enabled
#define g_fan2_on         g_actuation_state.fan2_on
#define g_fan2_en         g_actuation_state.fan2_enabled
#define g_relay_state     g_actuation_state.relay_on
#define g_safety_ovr      g_actuation_state.safety_override
#define g_emerg_stop      g_actuation_state.emergency_stop

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void actuation_data_init(void);

#endif // ACTUATION_DATA_H