// ============================================================================
// ACTUATION DATA IMPLEMENTATION
// ============================================================================

#include "actuation_data.h"

// ============================================================================
// GLOBAL STATE
// ============================================================================

actuation_state_t g_actuation_state = {
    .triac_power = 0,
    .triac_on = false,
    .fan1_speed = 0,
    .fan1_on = false,
    .fan2_on = false,
    .relay_on = false,
    .safety_override = false,
    .emergency_stop = false
};

// ============================================================================
// FUNCTION IMPLEMENTATIONS
// ============================================================================

void actuation_data_init(void) {
    g_actuation_state.triac_power = 0;
    g_actuation_state.triac_on = false;
    g_actuation_state.fan1_speed = 0;
    g_actuation_state.fan1_on = false;
    g_actuation_state.fan2_on = false;
    g_actuation_state.relay_on = false;
    g_actuation_state.safety_override = false;
    g_actuation_state.emergency_stop = false;
}