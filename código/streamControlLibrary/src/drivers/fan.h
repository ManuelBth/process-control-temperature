// ============================================================================
// fan.h - Header for fan PWM control
// ============================================================================

#ifndef FAN_H
#define FAN_H

#include <Arduino.h>

// Initialize fan PWM (both pins 16 and 17 share same duty cycle)
void fan_init();

// Set fan speed 0-100% (shared between both fans)
void fan_set_speed(float speed_percent);

#endif // FAN_H