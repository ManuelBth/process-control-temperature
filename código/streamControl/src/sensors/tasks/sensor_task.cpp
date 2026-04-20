// ============================================================================
// SENSOR TASK - Sensor reading task skeleton
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#include "sensor_task.h"
#include "sensor_config.h"
#include "sensor_data.h"

#include "task_manager.h"

// ============================================================================
// STATIC VARIABLES
// ============================================================================

static TickType_t xLastWakeTime;

// ============================================================================
// TASK IMPLEMENTATION
// ============================================================================

void vSensorTask(void *pvParameters) {
    // Initialize timing
    xLastWakeTime = xTaskGetTickCount();

    // Initialize sensor data
    sensor_data_init();

    // Main sensor reading loop
    while (1) {
        // Sensor reading logic will be implemented here

        // Delay until next sample period
        TASK_DELAY_UNTIL(&xLastWakeTime, SENSOR_READ_INTERVAL_MS);
    }
}