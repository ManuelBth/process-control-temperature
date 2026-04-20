// ============================================================================
// COMMUNICATION TASK - Header file
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef COMMUNICATION_TASK_H
#define COMMUNICATION_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FreeRTOSConfig.h"

// ============================================================================
// TASK HANDLE
// ============================================================================

extern TaskHandle_t xTaskCommunicationHandle;

// ============================================================================
// STACK SIZE
// ============================================================================

#define STACK_COMMUNICATION  512

// ============================================================================
// PRIORITY
// ============================================================================

#define PRIO_COMMUNICATION  (tskIDLE_PRIORITY + 3)

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void vCommunicationTask(void *pvParameters);

#endif // COMMUNICATION_TASK_H