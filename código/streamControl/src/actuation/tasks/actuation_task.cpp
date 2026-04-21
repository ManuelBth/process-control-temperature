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
#include "../rtos/handles.h"

// ============================================================================
// VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;
static percent_t s_power_read = 0;

// ============================================================================
// TAREA
// ============================================================================

void vActuationTask(void *pvParameters) {
    (void)pvParameters;

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

    Serial.println("[Actuation] Task started");

    // Loop principal
    while (1) {
        // Leer potencia del control (con mutex)
        if (xSemaphoreTake(mtx_actuation, pdMS_TO_TICKS(10)) == pdTRUE) {
            s_power_read = g_triac_pwr;
            xSemaphoreGive(mtx_actuation);
        }

        // Actualizar potencia del TRIAC
        // Nota: triac_process() lee de g_triac_pwr directamente

        // Verificar si está habilitado
        if (g_actuation_state.enabled) {
            // Verificar cruce por cero y procesar TRIAC
            if (zc_check()) {
                // Actualizar potencia antes de procesar
                triac_set_power(s_power_read);

                if (triac_process()) {
                    triac_fire();
                }
            }
        }

        // Pequeño delay para evitar busy-waiting
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
