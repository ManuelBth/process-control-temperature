// ============================================================================
// COMMUNICATION TASK - Serial communication task skeleton
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#include "communication_task.h"
#include "comm_config.h"
#include "comm_data.h"

#include "task_manager.h"

// ============================================================================
// STATIC VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;

// ============================================================================
// TASK IMPLEMENTATION
// ============================================================================

void vCommunicationTask(void *pvParameters) {
    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    // Initialize communication data
    comm_data_init();

    // Main communication loop
    while (1) {
        // Communication logic will be implemented here
        // - Read incoming commands
        // - Parse commands
        // - Execute commands
        // - Send responses

        // Delay until next sample period
        TASK_DELAY_UNTIL(&xLastWakeTime, 10);  // 10ms for responsive command handling
    }
}