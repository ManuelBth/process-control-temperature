// ============================================================================
// Handles - Task handles, queues, semaphores implementation
// ============================================================================

#include "handles.h"
#include <Arduino.h>

// ============================================================================
// TASK HANDLES - Initialize to NULL
// ============================================================================

TaskHandle_t xTaskSensorHandle = NULL;
TaskHandle_t xTaskControlHandle = NULL;
TaskHandle_t xTaskActuationHandle = NULL;

// ============================================================================
// MUTEX HANDLES - Initialize to NULL
// ============================================================================

SemaphoreHandle_t mtx_temperature = NULL;
SemaphoreHandle_t mtx_actuation = NULL;

// ============================================================================
// HANDLES_INIT - Initialize all handles
// ============================================================================

void handles_init(void) {
    Serial.println("[Handles] Initializing...");

    // Create mutex for temperature (g_temp)
    mtx_temperature = xSemaphoreCreateMutex();
    if (mtx_temperature != NULL) {
        Serial.println("[Handles] Created mtx_temperature");
    }

    // Create mutex for actuation (g_triac_pwr)
    mtx_actuation = xSemaphoreCreateMutex();
    if (mtx_actuation != NULL) {
        Serial.println("[Handles] Created mtx_actuation");
    }

    Serial.println("[Handles] Initialization complete");
}