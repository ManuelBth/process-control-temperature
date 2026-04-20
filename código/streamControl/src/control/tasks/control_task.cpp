// ============================================================================
// CONTROL TASK - Control loop task skeleton
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#include "control_task.h"
#include "control_config.h"
#include "control_data.h"

#include "task_manager.h"

// ============================================================================
// STATIC VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;

// ============================================================================
// TASK IMPLEMENTATION
// ============================================================================

void vControlTask(void *pvParameters) {
    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    // Initialize control data
    control_data_init();

    // Main control loop
    while (1) {
        // Control logic will be implemented here

        // Delay until next sample period
        TASK_DELAY_UNTIL(&xLastWakeTime, CONTROL_SAMPLE_RATE_MS);
    }
}