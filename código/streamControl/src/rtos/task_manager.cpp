// ============================================================================
// Task Manager - Task registration and startup logic
// ============================================================================

#include "task_manager.h"
#include <Arduino.h>
#include "handles.h"

// ============================================================================
// PUBLIC TASK HANDLES - Implemented in handles.h
// ============================================================================

// ============================================================================
// IDLE TASK - Runs when CPU is idle
// ============================================================================

void vTaskMonitor(void *pvParameters) {
    (void)pvParameters;  // Unused

    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint32_t tick_count = 0;

    Serial.println("[Monitor] Task started");

    for (;;) {
        // Log system status every 30 seconds
        if (tick_count % 300 == 0) {
            Serial.printf("[Monitor] Free heap: %u bytes\n", ESP.getFreeHeap());
            Serial.printf("[Monitor] Uptime: %lu seconds\n", millis() / 1000);
        }

        tick_count++;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}

// ============================================================================
// TASK MANAGER INIT - Register and start all tasks
// ============================================================================

void task_manager_init(void) {
    Serial.println("[TaskMgr] Initializing...");

    // Initialize handles first (queues, semaphores, etc.)
    handles_init();

    // Create monitor task
    xTaskCreate(
        vTaskMonitor,           // Task function
        "Monitor",            // Task name
        STACK_MONITOR,         // Stack depth (words)
        NULL,                 // Parameters
        PRIO_MONITOR,         // Priority
        &xTaskMonitorHandle   // Task handle
    );

    Serial.printf("[TaskMgr] Created Monitor task (handle: %p)\n",
                xTaskMonitorHandle);

    // TODO: Create other tasks as needed
    // - vTaskSensor for sensor reading
    // - vTaskControl for process control
    // - vTaskDisplay for UI updates
    // - vTaskComm for WiFi/MQTT

    Serial.println("[TaskMgr] Initialization complete");
}