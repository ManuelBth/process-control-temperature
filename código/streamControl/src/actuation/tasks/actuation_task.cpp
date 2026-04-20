// ============================================================================
// ACTUATION TASK - Actuation control task skeleton
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#include "actuation_task.h"
#include "actuation_config.h"
#include "actuation_data.h"

#include "task_manager.h"

// ============================================================================
// STATIC VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;

// ============================================================================
// TASK IMPLEMENTATION
// ============================================================================

void vActuationTask(void *pvParameters) {
    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    // Initialize actuation data
    actuation_data_init();

    // Main actuation loop 
    while (1) {
        // Actuation control logic will be implemented here

        // Delay until next update period
        TASK_DELAY_UNTIL(&xLastWakeTime, ACTUATION_UPDATE_INTERVAL_MS);
    }
}