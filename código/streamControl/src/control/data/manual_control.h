// ============================================================================
// MANUAL CONTROL - Control manual para prueba
// ============================================================================

#ifndef MANUAL_CONTROL_H
#define MANUAL_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// FUNCTIONS
// ============================================================================

// Inicializar control manual
void manual_init(void);

// Establecer ángulo de disparo (0-100%)
void manual_set_angle(uint8_t angle);

// Loop principal - procesar serial + control
void manual_loop(void);

// Verificar si está activo
bool manual_is_active(void);

// Activar/desactivar modo manual
void manual_enable(bool enable);

// Obtener setpoint actual
float manual_get_setpoint(void);

#endif // MANUAL_CONTROL_H