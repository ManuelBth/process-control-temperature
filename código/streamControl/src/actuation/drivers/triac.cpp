// ============================================================================
// TRIAC DRIVER - TRIAC firing control driver
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#include "actuation_config.h"
#include "common/central_data.h"

// ============================================================================
// TRIAC INITIALIZATION
// ============================================================================

void triac_init(void) {
    // TODO: Implement TRIAC initialization
    // - Configure zero-cross input pin
    // - Configure fire output pin
}

// ============================================================================
// TRIAC SET POWER
// ============================================================================

void triac_set_power(percent_t power) {
    // TODO: Implement TRIAC power setting
    // - Convert power to firing angle
    // - Set TRIAC firing angle
}

// ============================================================================
// TRIAC FIRE
// ============================================================================

void triac_fire(void) {
    // TODO: Implement TRIAC firing
    // - Wait for zero-cross
    // - Delay by firing angle
    // - Fire TRIAC pulse
}

// ============================================================================
// TRIAC ENABLE
// ============================================================================

void triac_enable(bool enable) {
    // TODO: Implement TRIAC enable/disable
}

// ============================================================================
// TRIAC GET POWER
// ============================================================================

percent_t triac_get_power(void) {
    // TODO: Implement TRIAC power get
    return 0;
}

// ============================================================================
// TRIAC IS ACTIVE
// ============================================================================

bool triac_is_active(void) {
    // TODO: Implement TRIAC active check
    return false;
}