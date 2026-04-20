// ============================================================================
// TRIAC CONTROL IMPLEMENTATION
// ============================================================================

#include "triac_control.h"
#include <Arduino.h>

// ============================================================================
// ESTADO
// ============================================================================

static percent_t s_power = 0;
static bool s_enabled = false;
static bool s_firing = false;

// ============================================================================
// FUNCIONES
// ============================================================================

void triac_init(void) {
    pinMode(PIN_TRIAC, OUTPUT);
    digitalWrite(PIN_TRIAC, LOW);
    s_power = 0;
    s_enabled = false;
    s_firing = false;
}

void triac_set_power(percent_t power) {
    if (power > 100) power = 100;
    s_power = power;
}

percent_t triac_get_power(void) {
    return s_power;
}

void triac_enable(void) {
    s_enabled = true;
}

void triac_disable(void) {
    s_enabled = false;
    digitalWrite(PIN_TRIAC, LOW);
    s_firing = false;
}

// Retorna true si es momento de disparar
bool triac_process(void) {
    if (!s_enabled) return false;
    if (s_power == 0) return false;
    
    // Calcular delay según potencia
    // 0% = delay máximo (no dispara)
    // 100% = delay mínimo (dispara inmediatamente)
    uint32_t delay_us = ((100 - s_power) * HALF_CYCLE_US) / 100;
    
    uint32_t time_since_zc = zc_time_since();
    
    return (time_since_zc >= delay_us);
}

void triac_fire(void) {
    if (s_firing) return;
    
    digitalWrite(PIN_TRIAC, HIGH);
    s_firing = true;
    delayMicroseconds(TRIAC_PULSE_US);
    digitalWrite(PIN_TRIAC, LOW);
    s_firing = false;
}
