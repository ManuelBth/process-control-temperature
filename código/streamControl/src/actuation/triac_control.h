// ============================================================================
// TRIAC CONTROL - Control de potencia por ángulo de fase
// ============================================================================

#ifndef TRIAC_CONTROL_H
#define TRIAC_CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include "pins.h"
#include "actuation_config.h"
#include "zero_cross.h"

// ============================================================================
// CONFIGURACIÓN
// ============================================================================

constexpr int TRIAC_PULSE_US = 50;    // Ancho de pulso de disparo (µs)

// ============================================================================
// FUNCIONES
// ============================================================================

void triac_init(void);
percent_t triac_get_power(void);
bool triac_process(void);  // Returns true si debe dispara
void triac_fire(void);

#endif
