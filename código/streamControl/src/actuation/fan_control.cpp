// ============================================================================
// FAN CONTROL IMPLEMENTATION
// ============================================================================

#include "fan_control.h"
#include <Arduino.h>

// ============================================================================
// FAN 1 (PWM)
// ============================================================================

static percent_t s_fan1_speed = 0;
static bool s_fan1_enabled = false;

void fan1_init(void) {
    pinMode(PIN_FAN1_PWM, OUTPUT);
    digitalWrite(PIN_FAN1_PWM, LOW);
    // TODO: configurar LEDC PWM
    s_fan1_speed = 0;
    s_fan1_enabled = false;
}

void fan1_set_speed(percent_t speed) {
    if (speed > 100) speed = 100;
    s_fan1_speed = speed;
    // TODO: escribir en LEDC
}

percent_t fan1_get_speed(void) {
    return s_fan1_speed;
}

void fan1_enable(void) {
    s_fan1_enabled = true;
}

void fan1_disable(void) {
    s_fan1_enabled = false;
    digitalWrite(PIN_FAN1_PWM, LOW);
}

// ============================================================================
// FAN 2 (ON/OFF)
// ============================================================================

static bool s_fan2_on = false;

void fan2_init(void) {
    pinMode(PIN_FAN2, OUTPUT);
    digitalWrite(PIN_FAN2, LOW);
    s_fan2_on = false;
}

void fan2_set_on(bool on) {
    s_fan2_on = on;
    digitalWrite(PIN_FAN2, on ? HIGH : LOW);
}

bool fan2_get(void) {
    return s_fan2_on;
}

// ============================================================================
// RELAY
// ============================================================================

static bool s_relay_on = false;

void relay_init(void) {
    pinMode(PIN_RELAY, OUTPUT);
    digitalWrite(PIN_RELAY, LOW);  // Asumiendo activo bajo
    s_relay_on = false;
}

void relay_set(bool on) {
    s_relay_on = on;
    digitalWrite(PIN_RELAY, on ? LOW : HIGH);
}

bool relay_get(void) {
    return s_relay_on;
}
