// ============================================================================
// ACTUATION TASK - Tarea de control de actuadores
// ============================================================================

#include "actuation_task.h"
#include "pins.h"

#include "zero_cross.h"
#include "triac_control.h"
#include "fan_control.h"

#include "task_manager.h"

// ============================================================================
// VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;

// ============================================================================
// TAREA
// ============================================================================

void vActuationTask(void *pvParameters) {
    xLastWakeTime = xTaskGetTickCount();

    // Inicializar drivers
    zc_init();
    triac_init();
    fan1_init();
    fan2_init();
    relay_init();

    // Habilitar sistema
    zc_enable();
    relay_set(true);

    // Loop principal
    while (1) {
        // Verificar cruce por cero
        if (zc_check()) {
            // Procesar TRIAC
            if (triac_process()) {
                triac_fire();
            }
        }

        // Pequeño delay para evitar busy-waiting
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
