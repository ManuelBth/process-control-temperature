// ============================================================================
// CONTROL TASK - Tarea del controlador
// ============================================================================

#ifndef CONTROL_TASK_H
#define CONTROL_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ============================================================================
// TASK CONFIG
// ============================================================================

#define STACK_CONTROL   512
#define PRIO_CONTROL   (tskIDLE_PRIORITY + 2)

// Task handle
extern TaskHandle_t xTaskControlHandle;

// ============================================================================
// FUNCTION
// ============================================================================

void vControlTask(void *pvParameters);

#endif // CONTROL_TASK_H