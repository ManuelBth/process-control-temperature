#ifndef ZERO_CROSS_H
#define ZERO_CROSS_H

#include <Arduino.h>

// Inicializar pin ZC e instalar ISR.
void zc_init();

// Actualizar potencia de forma atómica (seguro desde cualquier tarea).
// La ISR la leerá en el próximo cruce por cero.
void zc_set_power(uint8_t power_percent);

// Retorna true una sola vez por cada ZC detectado.
// Usar solo desde tareas, nunca desde ISR.
bool zc_detected();

// Período del último semiciclo válido en µs.
uint32_t zc_get_period_us();

// Contador total de interrupciones ZC desde el inicio.
uint32_t zc_get_isr_count();

// Variables compartidas (preferir los getters anteriores)
extern volatile bool     g_zc_event;
extern volatile uint32_t g_zc_period_us;
extern volatile uint32_t g_zc_isr_count;
extern volatile uint8_t  g_power_percent;

#endif // ZERO_CROSS_H