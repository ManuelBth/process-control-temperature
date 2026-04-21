// ============================================================================
// PINS - Definiciones de pines
// ============================================================================

#ifndef PINS_H
#define PINS_H

// ============================================================================
// TRIAC
// ============================================================================

constexpr int PIN_TRIAC = 16;           // Salida: disparo TRIAC
constexpr int PIN_ZERO_CROSS = 17;     // Entrada: detector cruce por cero

// ============================================================================
// VENTILADORES
// ============================================================================

constexpr int PIN_FAN1_PWM = 18;       // Salida: PWM ventilador 1
constexpr int PIN_FAN2 = 19;          // Salida: ON/OFF ventilador 2

// ============================================================================
// MAX6675 TERMOCOUPLE (VSPI)
// ============================================================================

constexpr int PIN_MAX6675_SCK = 22;     // Clock
constexpr int PIN_MAX6675_MISO = 25;    // Data output
constexpr int PIN_MAX6675_CS = 23;     // Chip select

// ============================================================================
// RELAY
// ============================================================================

constexpr int PIN_RELAY = 21;          // Salida: relay principal

// ============================================================================
// CONFIGURACIÓN ELÉCTRICA
// ============================================================================

constexpr int MAINS_FREQ_HZ = 50;      // Frecuencia red (Hz)
constexpr int HALF_CYCLE_US = 10000;  // Semi-período (10ms @ 50Hz)

#endif
