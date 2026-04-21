// ============================================================================
// Task Manager - Task registration and startup logic
// ============================================================================

#include "task_manager.h"
#include <Arduino.h>
#include "handles.h"

// Include task headers
#include "../sensors/tasks/sensor_task.h"
#include "../control/tasks/control_task.h"
#include "../actuation/tasks/actuation_task.h"

// ============================================================================
// TASK MANAGER INIT - Register and start all tasks
// ============================================================================

void task_manager_init(void) {
    Serial.println("[TaskMgr] Initializing...");

    // Initialize handles first (queues, semaphores, etc.)
    handles_init();

    // Create Sensor task (Core 0, highest priority)
    xTaskCreatePinnedToCore(
        vSensorTask,
        "Sensor",
        STACK_SENSOR,
        NULL,
        PRIO_SENSOR,
        &xTaskSensorHandle,
        0  // Core 0
    );
    Serial.printf("[TaskMgr] Created Sensor (handle: %p, core: 0)\n", xTaskSensorHandle);

    // Create Control task (Core 0)
    xTaskCreatePinnedToCore(
        vControlTask,
        "Control",
        STACK_CTRL,
        NULL,
        PRIO_CONTROL,
        &xTaskControlHandle,
        0  // Core 0
    );
    Serial.printf("[TaskMgr] Created Control (handle: %p, core: 0)\n", xTaskControlHandle);

    // Create Actuation task (Core 0)
    xTaskCreatePinnedToCore(
        vActuationTask,
        "Actuation",
        STACK_ACTUATION,
        NULL,
        PRIO_ACTUATION,
        &xTaskActuationHandle,
        0  // Core 0
    );
    Serial.printf("[TaskMgr] Created Actuation (handle: %p, core: 0)\n", xTaskActuationHandle);

    Serial.println("[TaskMgr] Initialization complete");
}