// ============================================================================
// SENSOR TASK - Tarea del sensor de temperatura
// ============================================================================

#include "sensor_task.h"
#include <Arduino.h>
#include "../data/sensor_data.h"
#include "../config/sensor_config.h"
#include "../rtos/handles.h"

// Enable MAX6675 for real temperature reading
#define MAX6675_ENABLED 1

#if MAX6675_ENABLED
#include "../drivers/max6675.h"
#endif

// ============================================================================
// TASK
// ============================================================================

void vSensorTask(void *pvParameters) {
    (void)pvParameters;

    Serial.println("[Sensor] Before init...");
    Serial.flush();
    delay(100);  // Small delay before doing anything
    
    Serial.println("[Sensor] Init start...");
    Serial.flush();

#if MAX6675_ENABLED
    max6675_init();
    Serial.println("[Sensor] max6675_init done");

    float test_temp = max6675_read();
    Serial.printf("[Sensor] Test read: %.2f\n", test_temp);
#else
    Serial.println("[Sensor] Task started (MAX6675 disabled for testing)");
#endif
    Serial.flush();

    for (;;) {
        uint32_t interval_ms = (g_sensor_mode == SENSOR_MODE_PID)
                                ? SENSOR_INTERVAL_PID_MS
                                : SENSOR_INTERVAL_NORMAL_MS;

#if MAX6675_ENABLED
        float temp = max6675_read();

        if (xSemaphoreTake(mtx_temperature, pdMS_TO_TICKS(10)) == pdTRUE) {
            if (temp != TEMP_ERROR) {
                g_temp = temp;
                g_sensor_err = false;
            } else {
                g_sensor_err = true;
            }
            g_sensor_state.last_read_ms = millis();
            xSemaphoreGive(mtx_temperature);
        }
#else
        // Simulate temperature reading for testing
        if (xSemaphoreTake(mtx_temperature, pdMS_TO_TICKS(10)) == pdTRUE) {
            g_temp = 25.0f + random(0, 100) / 10.0f;  // Fake temp 25-35°C
            g_sensor_err = false;
            g_sensor_state.last_read_ms = millis();
            xSemaphoreGive(mtx_temperature);
        }
#endif

        vTaskDelay(pdMS_TO_TICKS(interval_ms));
    }
}