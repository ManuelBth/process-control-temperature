#ifndef CONTROL_DRIVERS_TRIAC_H
#define CONTROL_DRIVERS_TRIAC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ─── Inicialización ──────────────────────────────────────────────────────────
// Configura el pin del MOC3021 como salida e instala el esp_timer
// de disparo. El gate permanece activo hasta el próximo ZC.
// Llamar una sola vez al inicio.
void triac_init();

// ─── Armar disparo por ángulo de fase ────────────────────────────────────────
// Programa el disparo del gate del TRIAC con el retardo correcto según
// el ángulo de fase calculado a partir de power_percent.
//
// Debe llamarse desde la ISR del cruce por cero (marcada IRAM_ATTR).
//
//   semiciclo_us  — duración del semiciclo en µs (viene del ZC detector)
//   power_percent — potencia deseada 0–100 (calculada por el PID)
//
// Lógica de disparo:
//   power = 100% → retardo = 0 µs         (dispara inmediato, máxima potencia)
//   power = 50%  → retardo = semiciclo/2   (dispara a mitad del semiciclo)
//   power = 0%   → no dispara             (TRIAC bloqueado)
void IRAM_ATTR triac_arm(uint32_t semiciclo_us, uint8_t power_percent);

#ifdef __cplusplus
}
#endif

#endif