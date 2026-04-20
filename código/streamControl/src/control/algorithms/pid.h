// ============================================================================
// PID ALGORITHM HEADER
// ============================================================================

#ifndef PID_H
#define PID_H

#include <stdint.h>

// ============================================================================
// FUNCTIONS
// ============================================================================

// Calcular salida PID
// temperature: temperatura actual
// setpoint: temperatura objetivo
// dt: delta tiempo en segundos
float pid_calculate(float temperature, float setpoint, float dt);

// Resetear estado del PID
void pid_reset(void);

#endif // PID_H