// ============================================================================
// ACTUATION TASK - Tarea de control de actuadores
// ============================================================================

#include "actuation_task.h"
#include <Arduino.h>
#include "../common/pins.h"
#include "../data/actuation_data.h"

#include "../zero_cross.h"
#include "../triac_control.h"
#include "../fan_control.h"

#include "../rtos/task_manager.h"
#include "../rtos/handles.h"

// Enable/disable drivers for testing
#define DRIVERS_ENABLED 0

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
    
    Serial.println("[Actuation] Before driver init...");
    Serial.flush();
    delay(100);

#if DRIVERS_ENABLED
    Serial.println("[Actuation] Init zc...");
    zc_init();
    
    Serial.println("[Actuation] Init triac...");
    triac_init();
    
    Serial.println("[Actuation] Init fans...");
    fan1_init();
    fan2_init();
    
    Serial.println("[Actuation] Init relay...");
    relay_init();
    
    Serial.println("[Actuation] Enable zc...");
    zc_enable();
    relay_set(true);
#else
    Serial.println("[Actuation] Drivers disabled for test...");
#endif
    
    g_actuation_state.running = true;
    Serial.println("[Actuation] Task started");
    Serial.flush();

    // Loop principal
    while (1) {
        // Read power from control (with mutex)
        if (xSemaphoreTake(mtx_actuation, pdMS_TO_TICKS(10)) == pdTRUE) {
            s_power_read = g_triac_pwr;
            xSemaphoreGive(mtx_actuation);
        }

#if DRIVERS_ENABLED
        if (g_actuation_state.enabled) {
            if (zc_check()) {
                triac_set_power(s_power_read);
                if (triac_process()) {
                    triac_fire();
                }
            }
        }
#else
        // Log status for debug
        static uint32_t last_log = 0;
        if (millis() - last_log > 3000) {
            last_log = millis();
            Serial.printf("[Actuation] Power: %d, enabled: %d\n", s_power_read, g_actuation_state.enabled);
        }
#endif

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
