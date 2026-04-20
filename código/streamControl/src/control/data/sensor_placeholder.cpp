// ============================================================================
// SENSOR PLACEHOLDER IMPLEMENTATION
// ============================================================================

#include "sensor_placeholder.h"

// Temperatura actual (placeholder)
float g_pt100_temperature = 25.0f;  // 25°C por defecto

// ============================================================================
// FUNCTIONS
// ============================================================================

void sensor_init(void) {
    // TODO: inicializar ADC para PT100
    g_pt100_temperature = 25.0f;
}

float sensor_read(void) {
    // TODO: leer ADC y convertir a temperatura
    // Por ahora retorna el valor actual
    return g_pt100_temperature;
}