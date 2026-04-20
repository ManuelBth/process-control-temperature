// ============================================================================
// SENSOR PLACEHOLDER - Temporal hasta implementar sensor PT100
// ============================================================================

#ifndef SENSOR_PLACEHOLDER_H
#define SENSOR_PLACEHOLDER_H

#include <stdint.h>

// Temperatura actual (placeholder - reemplazar con lectura real)
extern float g_pt100_temperature;

// Inicializar sensor (placeholder)
void sensor_init(void);

// Leer temperatura (placeholder - reemplazar con lectura real)
float sensor_read(void);

#endif // SENSOR_PLACEHOLDER_H