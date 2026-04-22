// ============================================================================
// TRIAC CONTROL IMPLEMENTATION
// ============================================================================

#include "triac_control.h"
#include "data/actuation_data.h"
#include <Arduino.h>

// ============================================================================
// ESTADO
// ============================================================================

static percent_t s_power = 0;
static bool s_firing = false;

// ============================================================================
// FUNCIONES
// ============================================================================

void triac_init(void) {
    pinMode(PIN_TRIAC, OUTPUT);
    digitalWrite(PIN_TRIAC, LOW);
    s_power = 0;
    s_firing = false;
}

void triac_set_power(percent_t power) {
    s_power = power;
}

percent_t triac_get_power(void) {
    return s_power;
}

// Retorna true si es momento de dispara
bool triac_process(void) {
    if (s_power == 0) return false;

    // Mapear potencia (0-100%) a ángulo (30-150°)
    // 0% potencia → 150° (min power, max delay)
    // 100% potencia → 30° (max power, min delay)
    uint8_t angle = TRIAC_ANGLE_MIN + ((100 - s_power) * (TRIAC_ANGLE_MAX - TRIAC_ANGLE_MIN)) / 100;

    // Convertir ángulo a delay: ángulo/180 * 10000µs
    uint32_t delay_us = (angle * HALF_CYCLE_US) / 180;

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
