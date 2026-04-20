// ============================================================================
// ZERO CROSS IMPLEMENTATION
// ============================================================================

#include "zero_cross.h"
#include <Arduino.h>

// ============================================================================
// ESTADO
// ============================================================================

zero_cross_state_t g_zc = {
    .detected = false,
    .last_time_us = 0,
    .cycle_count = 0,
    .enabled = false
};

// ============================================================================
// FUNCIONES
// ============================================================================

void zc_init(void) {
    pinMode(PIN_ZERO_CROSS, INPUT);
    g_zc.detected = false;
    g_zc.last_time_us = 0;
    g_zc.cycle_count = 0;
    g_zc.enabled = false;
}

void zc_enable(void) {
    if (!g_zc.enabled) {
        attachInterrupt(PIN_ZERO_CROSS, zc_isr, ZC_INTERRUPT_MODE);
        g_zc.enabled = true;
    }
}

void zc_disable(void) {
    if (g_zc.enabled) {
        detachInterrupt(PIN_ZERO_CROSS);
        g_zc.enabled = false;
    }
}

bool zc_check(void) {
    if (g_zc.detected) {
        g_zc.detected = false;
        return true;
    }
    return false;
}

uint32_t zc_time_since(void) {
    return micros() - g_zc.last_time_us;
}

uint32_t zc_get_count(void) {
    return g_zc.cycle_count;
}

// ============================================================================
// ISR
// ============================================================================

void IRAM_ATTR zc_isr(void) {
    g_zc.detected = true;
    g_zc.last_time_us = micros();
    g_zc.cycle_count++;
}
