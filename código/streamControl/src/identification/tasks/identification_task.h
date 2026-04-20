// ============================================================================
// IDENTIFICATION TASK - Header file
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef IDENTIFICATION_TASK_H
#define IDENTIFICATION_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "FreeRTOSConfig.h"

// ============================================================================
// TASK HANDLE
// ============================================================================

extern TaskHandle_t xTaskIdentificationHandle;

// ============================================================================
// STACK SIZE
// ============================================================================

#define STACK_IDENTIFICATION  384

// ============================================================================
// PRIORITY
// ============================================================================

#define PRIO_IDENTIFICATION  (tskIDLE_PRIORITY + 1)

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void vIdentificationTask(void *pvParameters);

#endif // IDENTIFICATION_TASK_H