// ============================================================================
// fan.cpp - Control PWM de ventiladores
// ============================================================================
// both fans share the same PWM - same duty cycle on pins 16 and 17
// ============================================================================

#include <Arduino.h>
#include <driver/ledc.h>
#include "common/pins.h"
#include "common/data.h"
#include "drivers/fan.h"

// LEDC configuration
#define FAN_LEDC_CHANNEL  2        // Canal LEDC dedicado para ventiladores
#define FAN_PWM_FREQ       25000    // 25 kHz para ventiladores

// ============================================================================
// Inicializar PWM de ventiladores
// ============================================================================
void fan_init() {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    // API LEDC moderna (core 3.0+): configura y atacha en un solo paso
    ledcAttach(PIN_FAN1, FAN_PWM_FREQ, 8);
    ledcAttach(PIN_FAN2, FAN_PWM_FREQ, 8);
#else
    // API LEDC legacy (core 2.x): setup + attach por canal
    ledcSetup(FAN_LEDC_CHANNEL, FAN_PWM_FREQ, 8);
    ledcAttachPin(PIN_FAN1, FAN_LEDC_CHANNEL);
    ledcAttachPin(PIN_FAN2, FAN_LEDC_CHANNEL);
#endif

    // Arrancar inmediatamente con velocidad base para homogeneización de aire
    fan_set_speed(FAN_SPEED_NORMAL);

    Serial.printf("Fan PWM init: PIN_FAN1=%d, PIN_FAN2=%d, freq=%dHz\n",
                 PIN_FAN1, PIN_FAN2, FAN_PWM_FREQ);
}

// ============================================================================
// Set fan speed (0-100%)
// ============================================================================
void fan_set_speed(float speed_percent) {
    if (speed_percent < 0.0f) speed_percent = 0.0f;
    if (speed_percent > 100.0f) speed_percent = 100.0f;

    // Convert 0-100% to 0-255 (8-bit duty cycle)
    uint8_t duty = (uint8_t)((speed_percent / 100.0f) * 255.0f);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(PIN_FAN1, duty);
    ledcWrite(PIN_FAN2, duty);
#else
    ledcWrite(FAN_LEDC_CHANNEL, duty);
#endif
}

// ============================================================================
// Set perturbation mode
//   true  → FAN_SPEED_PERTURBATION (100%)
//   false → FAN_SPEED_NORMAL (50%)
// ============================================================================
void fan_set_perturbation(bool active) {
    float speed = active ? FAN_SPEED_PERTURBATION : FAN_SPEED_NORMAL;
    g_data.fan_speed = speed;
    fan_set_speed(speed);
}