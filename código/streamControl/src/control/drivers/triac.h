#ifndef TRIAC_H
#define TRIAC_H

#include <Arduino.h>

// Inicializar GPIO y esp_timer del gate.
// Llamar una sola vez al inicio.
void triac_init();

// Armar el disparo del gate tras el delay calculado.
// Llamar SOLO desde la ISR del ZC (es IRAM_ATTR).
//
//   semiciclo_us  — período del semiciclo medido (µs)
//   power_percent — potencia deseada 0–100
void IRAM_ATTR triac_arm(uint32_t semiciclo_us, uint8_t power_percent);

#endif // TRIAC_H