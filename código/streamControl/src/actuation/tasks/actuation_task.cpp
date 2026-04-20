// ============================================================================
// ACTUATION TASK - Tarea de control de actuadores
// ============================================================================

#include "actuation_task.h"
#include "pins.h"
#include "actuation_data.h"

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

    // Señalar que la tarea está corriendo
    g_actuation_state.running = true;

    // Loop principal
    while (1) {
        // Verificar si está habilitado
        if (!g_actuation_state.enabled) {
            // Esperar hasta que se habilite
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        // Verificar cruce por cero y procesar TRIAC inmediatamente
        if (zc_check()) {
            if (triac_process()) {
                triac_fire();
            }
        }

        // Pequeño delay para evitar busy-waiting
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
