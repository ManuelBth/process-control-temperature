// ============================================================================
// ACTUATION TASK - Tarea de control de actuadores
// ============================================================================

#include "actuation_task.h"
#include "central_data.h"
#include "pins.h"

#include "zero_cross.h"
#include "triac_control.h"
#include "fan_control.h"

#include "task_manager.h"

// ============================================================================
// VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;

// Inicializar datos globales
control_data_t g_control = {
    .mode = CONTROL_OFF,
    .setpoint = 0,
    .temperature = 0,
    .kp = 1.0,
    .ki = 0.1,
    .kd = 0.01,
    .output = 0
};

actuator_data_t g_actuator = {
    .triac_power = 0,
    .triac_enabled = false,
    .fan1_speed = 0,
    .fan1_enabled = false,
    .fan2_on = false,
    .relay_on = false
};

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
    triac_enable();
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
