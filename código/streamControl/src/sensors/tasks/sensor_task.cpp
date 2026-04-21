// ============================================================================
// SENSOR TASK - Tarea del sensor de temperatura
// ============================================================================

#include "sensor_task.h"
#include "sensor_data.h"
#include "sensor_config.h"
#include "drivers/max6675.h"
#include "../rtos/handles.h"

// ============================================================================
// TASK
// ============================================================================

void vSensorTask(void *pvParameters) {
    (void)pvParameters;

    // Inicializar driver
    max6675_init();
    Serial.println("[Sensor] Task started");

    for (;;) {
        // Determinar intervalo según modo
        uint32_t interval_ms = (g_sensor_mode == SENSOR_MODE_PID)
                                ? SENSOR_INTERVAL_PID_MS
                                : SENSOR_INTERVAL_NORMAL_MS;

        // Leer temperatura
        float temp = max6675_read();

        // Tomar mutex y escribir
        if (xSemaphoreTake(mtx_temperature, pdMS_TO_TICKS(10)) == pdTRUE) {
            // Verificar lectura válida
            if (temp != TEMP_ERROR) {
                g_temp = temp;
                g_sensor_err = false;
            } else {
                g_sensor_err = true;
            }
            g_sensor_state.last_read_ms = millis();

            xSemaphoreGive(mtx_temperature);
        }

        // Timing
        vTaskDelay(pdMS_TO_TICKS(interval_ms));
    }
}