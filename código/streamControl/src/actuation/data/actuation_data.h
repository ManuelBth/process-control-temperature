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
    bool triac_on;

    // FAN1 (PWM)
    percent_t fan1_speed;
    bool fan1_on;

    // FAN2 (ON/OFF)
    bool fan2_on;

    // Main relay
    bool relay_on;

    // Safety
    bool safety_override;
    bool emergency_stop;
} actuation_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern actuation_state_t g_actuation_state;

// ============================================================================
// ACCESS MACROS
// ============================================================================

#define g_triac_pwr       g_actuation_state.triac_power
#define g_triac_on        g_actuation_state.triac_on
#define g_fan1_spd        g_actuation_state.fan1_speed
#define g_fan1_on         g_actuation_state.fan1_on
#define g_fan2_on         g_actuation_state.fan2_on
#define g_relay_on        g_actuation_state.relay_on
#define g_safety_ovr      g_actuation_state.safety_override
#define g_emerg_stop      g_actuation_state.emergency_stop

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void actuation_data_init(void);

#endif // ACTUATION_DATA_H