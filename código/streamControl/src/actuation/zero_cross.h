// ============================================================================
// ZERO CROSS DETECTOR - Detecta cruce por cero de la red AC
// ============================================================================

#ifndef ZERO_CROSS_H
#define ZERO_CROSS_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

// Pines
#include "pins.h"

// ============================================================================
// CONFIGURACIÓN
// ============================================================================

// Modo de interrupción: FALLING para optocoplador que sale LOW en cruce por cero
constexpr int ZC_INTERRUPT_MODE = FALLING;

// ============================================================================
// ESTADO
// ============================================================================

typedef struct {
    volatile bool detected;           // Flag: cruce por cero detectado
    volatile uint32_t last_time_us;  // Timestamp del último cruce
    volatile uint32_t cycle_count;   // Contador de semi-ciclos
    bool enabled;
} zero_cross_state_t;

extern zero_cross_state_t g_zc;

// ============================================================================
// FUNCIONES
// ============================================================================

void zc_init(void);
void zc_enable(void);
void zc_disable(void);
bool zc_check(void);
uint32_t zc_time_since(void);
uint32_t zc_get_count(void);
void IRAM_ATTR zc_isr(void);  // ISR

#endif
