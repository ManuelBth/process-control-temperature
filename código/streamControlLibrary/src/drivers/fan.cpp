// ============================================================================
// fan.cpp - Control PWM de ventiladores
// ============================================================================
// both fans share the same PWM - same duty cycle on pins 16 and 17
// ============================================================================

#include <Arduino.h>
#include <driver/ledc.h>
#include "common/pins.h"

// LEDC configuration
#define FAN_LEDC_CHANNEL  2        // Canal LEDC 2 (0 y 1 usados por ZC/TRIAC)
#define FAN_LEDC_TIMER     0        // Timer 0
#define FAN_PWM_FREQ       25000    // 25 kHz para ventiladores

// ============================================================================
// Inicializar PWM de ventiladores
// ============================================================================
void fan_init() {
    // Configurar canal LEDC: 25kHz, 8 bits de resolución
    ledcSetup(FAN_LEDC_CHANNEL, FAN_PWM_FREQ, 8);
    
    // Adjuntar ambos pines al mismo canal
    ledcAttachPin(PIN_FAN1, FAN_LEDC_CHANNEL);
    ledcAttachPin(PIN_FAN2, FAN_LEDC_CHANNEL);
    
    // Iniciar con duty cycle 0 (apagados)
    ledcWrite(FAN_LEDC_CHANNEL, 0);
    
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
    ledcWrite(FAN_LEDC_CHANNEL, duty);
}