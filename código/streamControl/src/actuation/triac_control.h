// ============================================================================
// TRIAC CONTROL - Control de potencia por ángulo de fase
// ============================================================================

#ifndef TRIAC_CONTROL_H
#define TRIAC_CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include "pins.h"
#include "central_data.h"
#include "zero_cross.h"

// ============================================================================
// CONFIGURACIÓN
// ============================================================================

constexpr int TRIAC_PULSE_US = 50;    // Ancho de pulso de disparo (µs)

// ============================================================================
// FUNCIONES
// ============================================================================

void triac_init(void);
void triac_set_power(percent_t power);
percent_t triac_get_power(void);
void triac_enable(void);
void triac_disable(void);
bool triac_process(void);  // Returns true si debe dispara
void triac_fire(void);

#endif
