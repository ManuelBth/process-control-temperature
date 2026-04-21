// ============================================================================
// SENSOR DATA - Estado del sensor de temperatura
// ============================================================================

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "sensor_config.h"

// ============================================================================
// SENSOR STATE
// ============================================================================

typedef struct {
    // Temperatura actual (°C)
    float temperature;

    // Modo de operación
    sensor_mode_t mode;

    // Intervalo actual (ms)
    uint16_t interval_ms;

    // Habilitación
    volatile bool enabled;

    // Error flag
    bool error;

    // Última lectura timestamps
    uint32_t last_read_ms;
} sensor_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern sensor_state_t g_sensor_state;

// ============================================================================
// MACROS
// ============================================================================

#define g_temp         g_sensor_state.temperature
#define g_sensor_mode g_sensor_state.mode
#define g_sensor_en   g_sensor_state.enabled
#define g_sensor_err g_sensor_state.error

// ============================================================================
// FUNCTIONS
// ============================================================================

void sensor_data_init(void);

#endif // SENSOR_DATA_H