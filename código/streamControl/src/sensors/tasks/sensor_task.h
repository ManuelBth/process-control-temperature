// ============================================================================
// SENSOR TASK - Tarea del sensor de temperatura
// ============================================================================

#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ============================================================================
// TASK CONFIG
// ============================================================================

#define STACK_SENSOR   256
#define PRIO_SENSOR   (tskIDLE_PRIORITY + 3)

// Task handle
extern TaskHandle_t xTaskSensorHandle;

// ============================================================================
// FUNCTION
// ============================================================================

void vSensorTask(void *pvParameters);

#endif // SENSOR_TASK_H