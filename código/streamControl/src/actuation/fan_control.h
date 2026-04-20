// ============================================================================
// FAN CONTROL - Control de ventiladores
// ============================================================================

#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include "pins.h"
#include "actuation_config.h"

// ============================================================================
// MAPEO PWM
// ============================================================================

// Mapeo: velocidad 0-100% → PWM 0-255
#define FAN1_PWM_MAP(speed) ((uint8_t)(((speed) * 255) / 100))

// ============================================================================
// FUNCIONES
// ============================================================================

// Ventilador 1: PWM
void fan1_init(void);
void fan1_set_speed(percent_t speed);
percent_t fan1_get_speed(void);

// Ventilador 2: ON/OFF
void fan2_init(void);
void fan2_set_on(bool on);
bool fan2_get(void);

// Relay principal
void relay_init(void);
void relay_set(bool on);
bool relay_get(void);

#endif
