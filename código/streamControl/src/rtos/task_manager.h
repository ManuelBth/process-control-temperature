// ============================================================================
// Task Manager - TaskHandle_t definitions and helper macros
// ============================================================================

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FreeRTOSConfig.h"

// ============================================================================
// TASK CONFIGURATION - Stack sizes (in words, 4 bytes each)
// ============================================================================

// Minimum stack size for all tasks
#define STACK_MINIMAL    configMINIMAL_STACK_SIZE

// Stack sizes - smaller for ESP32 Arduino
// These are much smaller to fit in memory
#define STACK_SENSOR   1024    // Sensor task stack
#define STACK_CTRL     1024    // Control task stack  
#define STACK_ACTUATION 1024  // Actuation task stack

// ============================================================================
// PRIORITY CONFIGURATION - Based on task criticality
// ============================================================================

// IDLE priority (runs when nothing else)
#define PRIO_IDLE     tskIDLE_PRIORITY

// Actuation - TRIAC control (low-medium)
#define PRIO_ACTUATION (tskIDLE_PRIORITY + 1)

// Control - PID control (medium)
#define PRIO_CONTROL  (tskIDLE_PRIORITY + 2)

// Sensors - Data acquisition (high)
#define PRIO_SENSOR  (tskIDLE_PRIORITY + 3)

// ============================================================================
// HELPER MACROS
// ============================================================================

// Convert milliseconds to ticks
#define MS_TO_TICKS(ms) pdMS_TO_TICKS(ms)

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

#endif // TASK_MANAGER_H