// ============================================================================
// FAN CONTROL IMPLEMENTATION
// ============================================================================

#include "fan_control.h"
#include <Arduino.h>

// ============================================================================
// FAN 1 (PWM)
// ============================================================================

static percent_t s_fan1_speed = 0;

void fan1_init(void) {
    // Configurar LEDC: canal 0, pin FAN1, 8 bits, 1kHz
    ledcSetup(0, 1000, 8);
    ledcAttachPin(PIN_FAN1_PWM, 0);
    ledcWrite(0, 0);
    s_fan1_speed = 0;
}

void fan1_set_speed(percent_t speed) {
    if (speed > 100) speed = 100;
    s_fan1_speed = speed;
    uint8_t pwm_duty = FAN1_PWM_MAP(speed);
    ledcWrite(0, pwm_duty);
}

percent_t fan1_get_speed(void) {
    return s_fan1_speed;
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
