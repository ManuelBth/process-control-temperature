// ============================================================================
// Handles - Task handles, queues, semaphores
// ============================================================================

#ifndef HANDLES_H
#define HANDLES_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

// ============================================================================
// TASK HANDLES - Handles for all FreeRTOS tasks
// ============================================================================

extern TaskHandle_t xTaskSensorHandle;
extern TaskHandle_t xTaskControlHandle;
extern TaskHandle_t xTaskActuationHandle;

// ============================================================================
// QUEUE HANDLES - Handles for inter-task queues (not used for now)
// ============================================================================

// Queue handles can be added later if needed

// ============================================================================
// MUTEX HANDLES - For shared data synchronization
// ============================================================================

extern SemaphoreHandle_t mtx_temperature;   // Protects g_temp
extern SemaphoreHandle_t mtx_actuation;  // Protects g_triac_pwr

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Initialize all handles
void handles_init(void);

#endif // HANDLES_H