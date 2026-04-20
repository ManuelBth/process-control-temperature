// ============================================================================
// Task Manager - TaskHandle_t definitions and helper macros
// ============================================================================

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FreeRTOSConfig.h"

// ============================================================================
// TASK HANDLES - Handles for all FreeRTOS tasks
// ============================================================================

// Forward declarations for task handles (extern in handles.h)
extern TaskHandle_t xTaskMonitorHandle;

// ============================================================================
// TASK CONFIGURATION - Stack sizes (in words, 4 bytes each)
// ============================================================================

// Minimum stack size for all tasks
#define STACK_MINIMAL    configMINIMAL_STACK_SIZE

// Stack sizes for application tasks
#define STACK_MONITOR   256
#define STACK_SENSOR  256
#define STACK_CTRL   384
#define STACK_DISP   256
#define STACK_COMM   512

// ============================================================================
// PRIORITY CONFIGURATION - Based on task criticality
// ============================================================================

// IDLE priority (runs when nothing else)
#define PRIO_IDLE     tskIDLE_PRIORITY

// Monitor - Debug/logging (lowest)
#define PRIO_MONITOR (tskIDLE_PRIORITY + 0)

// Display - UI updates (low)
#define PRIO_DISPLAY (tskIDLE_PRIORITY + 0)

// Control - Process control (medium)
#define PRIO_CONTROL (tskIDLE_PRIORITY + 1)

// Sensors - Data acquisition (high)
#define PRIO_SENSOR   (tskIDLE_PRIORITY + 2)

// Communication - WiFi/MQTT (highest)
#define PRIO_COMM   (tskIDLE_PRIORITY + 3)

// ============================================================================
// HELPER MACROS
// ============================================================================

// Convert milliseconds to ticks
#define MS_TO_TICKS(ms) pdMS_TO_TICKS(ms)

// Create a task with standard parameters
#define TASK_CREATE(pxTaskCode, pcName, usStackDepth, pvParams, uxPriority, pxCreatedTask) \
    xTaskCreate( \
        (TaskFunction_t)(pxTaskCode), \
        (const char*)(pcName), \
        (configSTACK_DEPTH_TYPE)(usStackDepth), \
        (void*)(pvParams), \
        (UBaseType_t)(uxPriority), \
        (TaskHandle_t*)(pxCreatedTask) \
    )

// Delete current task
#define TASK_DELETE() vTaskDelete(NULL)

// Suspend current task
#define TASK_SUSPEND() vTaskSuspend(NULL)

// Resume a task
#define TASK_RESUME(pxTask) vTaskResume(pxTask)

// Delay task
#define TASK_DELAY(ms) vTaskDelay(MS_TO_TICKS(ms))

// Delay until (fixed rate)
#define TASK_DELAY_UNTIL(pxLastWake, ms) vTaskDelayUntil(pxLastWake, MS_TO_TICKS(ms))

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Initialize all tasks
void task_manager_init(void);

// Task function declarations (to be implemented)
void vTaskMonitor(void *pvParameters);

#endif // TASK_MANAGER_H