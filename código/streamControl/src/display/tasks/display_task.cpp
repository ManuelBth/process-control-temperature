// ============================================================================
// DISPLAY TASK - Display update task skeleton
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#include "display_task.h"
#include "display_config.h"
#include "display_data.h"

#include "task_manager.h"

// ============================================================================
// STATIC VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;

// ============================================================================
// TASK IMPLEMENTATION
// ============================================================================

void vDisplayTask(void *pvParameters) {
    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    // Initialize display data
    display_data_init();

    // Main display loop
    while (1) {
        // Display update logic will be implemented here
        // - Check if update needed
        // - Format data to buffer
        // - Send to LCD

        // Delay until next refresh period
        TASK_DELAY_UNTIL(&xLastWakeTime, DISPLAY_REFRESH_RATE_MS);
    }
}