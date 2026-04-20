// ============================================================================
// Handles - Task handles, queues, semaphores implementation
// ============================================================================

#include "handles.h"
#include <Arduino.h>

// ============================================================================
// TASK HANDLES - Initialize to NULL
// ============================================================================

TaskHandle_t xTaskMonitorHandle = NULL;
TaskHandle_t xTaskSensorHandle = NULL;
TaskHandle_t xTaskControlHandle = NULL;
TaskHandle_t xTaskDisplayHandle = NULL;
TaskHandle_t xTaskCommHandle = NULL;

// ============================================================================
// QUEUE HANDLES - Initialize to NULL (created in handles_init)
// ============================================================================

QueueHandle_t q_sensor_data = NULL;
QueueHandle_t q_control_cmd = NULL;
QueueHandle_t q_display_data = NULL;
QueueHandle_t q_comm_data = NULL;

// ============================================================================
// SEMAPHORE HANDLES - Initialize to NULL
// ============================================================================

SemaphoreHandle_t sem_data_ready = NULL;
SemaphoreHandle_t sem_system_ready = NULL;
SemaphoreHandle_t mtx_system_data = NULL;

// ============================================================================
// HANDLES_INIT - Initialize all handles
// ============================================================================

void handles_init(void) {
    Serial.println("[Handles] Initializing...");

    // TODO: Create actual queues when needed
    // Example:
    // q_sensor_data = xQueueCreate(10, sizeof(sensor_data_t));

    // TODO: Create actual semaphores when needed
    // Example:
    // sem_data_ready = xSemaphoreCreateBinary();
    // mtx_system_data = xSemaphoreCreateMutex();

    Serial.println("[Handles] Initialization complete (stubs)");
}