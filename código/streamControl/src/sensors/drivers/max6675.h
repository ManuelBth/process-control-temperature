// ============================================================================
// MAX6675 DRIVER - Driver para termocouple MAX6675
// ============================================================================

#ifndef MAX6675_H
#define MAX6675_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// FUNCTIONS
// ============================================================================

// Inicializar SPI y pines
void max6675_init(void);

// Leer temperatura (°C)
// Retorna TEMP_ERROR si hay error
float max6675_read(void);

// Verificar si hay error de termocuple
bool max6675_is_fault(void);

#endif // MAX6675_H