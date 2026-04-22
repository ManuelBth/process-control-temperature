// ============================================================================
// Task Manager - Task registration and startup logic
// ============================================================================

#include "task_manager.h"
#include <Arduino.h>
#include "handles.h"

// Forward declarations
extern void vSensorTask(void *pvParameters);
extern void vControlTask(void *pvParameters);
extern void vActuationTask(void *pvParameters);

// ============================================================================
// TASK MANAGER INIT - Register and start all tasks
// ============================================================================

void task_manager_init(void) {
    Serial.println("[TaskMgr] Initializing...");
    Serial.flush();

    // Initialize handles (creates mutexes) 
    handles_init();
    Serial.println("[TaskMgr] Handles init done");
    Serial.flush();
    
    delay(100);

    // Create Sensor task on Core 1
    Serial.println("[TaskMgr] Creating Sensor on Core 1...");
    Serial.flush();
    BaseType_t result = xTaskCreatePinnedToCore(
        vSensorTask,
        "Sensor",
        2048,
        NULL,
        5,
        &xTaskSensorHandle,
        1  // Core 1
    );
    Serial.printf("[TaskMgr] Sensor result: %d, handle: %p\n", result, xTaskSensorHandle);
    Serial.flush();
    if (result != pdPASS) {
        Serial.println("[TaskMgr] ERROR creating Sensor!");
        return;
    }
    
    delay(100);  // Wait before next task

    // Create Control task on Core 1
    Serial.println("[TaskMgr] Creating Control on Core 1...");
    Serial.flush();
    result = xTaskCreatePinnedToCore(
        vControlTask,
        "Control",
        2048,
        NULL,
        4,  // Slightly lower priority
        &xTaskControlHandle,
        1  // Core 1
    );
    Serial.printf("[TaskMgr] Control result: %d, handle: %p\n", result, xTaskControlHandle);
    Serial.flush();
    if (result != pdPASS) {
        Serial.println("[TaskMgr] ERROR creating Control!");
        return;
    }
    
    delay(100);  // Wait before next task

    // Create Actuation task on Core 1
    Serial.println("[TaskMgr] Creating Actuation on Core 1...");
    Serial.flush();
    result = xTaskCreatePinnedToCore(
        vActuationTask,
        "Actuation",
        2048,
        NULL,
        3,  // Lowest priority
        &xTaskActuationHandle,
        1  // Core 1
    );
    Serial.printf("[TaskMgr] Actuation result: %d, handle: %p\n", result, xTaskActuationHandle);
    Serial.flush();
    if (result != pdPASS) {
        Serial.println("[TaskMgr] ERROR creating Actuation!");
        return;
    }

    Serial.println("[TaskMgr] All tasks created successfully!");
    Serial.flush();
}