// ============================================================================
// Handles - Task handles, queues, semaphores (stubs for now)
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

extern TaskHandle_t xTaskMonitorHandle;
extern TaskHandle_t xTaskSensorHandle;
extern TaskHandle_t xTaskControlHandle;
extern TaskHandle_t xTaskDisplayHandle;
extern TaskHandle_t xTaskCommHandle;

// ============================================================================
// QUEUE HANDLES - Handles for inter-task queues
// ============================================================================

extern QueueHandle_t q_sensor_data;
extern QueueHandle_t q_control_cmd;
extern QueueHandle_t q_display_data;
extern QueueHandle_t q_comm_data;

// ============================================================================
// SEMAPHORE HANDLES - Handles for synchronization
// ============================================================================

extern SemaphoreHandle_t sem_data_ready;
extern SemaphoreHandle_t sem_system_ready;
extern SemaphoreHandle_t mtx_system_data;

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

// Initialize all handles
void handles_init(void);

#endif // HANDLES_H