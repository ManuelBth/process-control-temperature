// ============================================================================
// fan.h - Header for fan PWM control
// ============================================================================

#ifndef FAN_H
#define FAN_H

#include <Arduino.h>

// ---------------------------------------------------------------------------
// Constantes de velocidad del ventilador
// ---------------------------------------------------------------------------
constexpr float FAN_SPEED_NORMAL       = 70.0f;   // Flujo de aire base (homogeneización)
constexpr float FAN_SPEED_PERTURBATION = 100.0f;  // Perturbación térmica (enfriamiento)

// Initialize fan PWM (both pins 16 and 17 share same duty cycle)
void fan_init();

// Set fan speed 0-100% (shared between both fans)
void fan_set_speed(float speed_percent);

// Set perturbation mode: true = 100%, false = FAN_SPEED_NORMAL (20%)
// Actualiza g_data.fan_speed internamente.
void fan_set_perturbation(bool active);

#endif // FAN_H