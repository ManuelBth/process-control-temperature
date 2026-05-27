// ============================================================================
// LoggerTask - System status logging every 1 second
// ============================================================================

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "control/task/logger_task.h"
#include "common/data.h"

extern ControlData g_data;

void LoggerTask(void* parameter) {
    (void)parameter;

    Serial.println("LoggerTask started");
    Serial.flush();

    uint32_t last_log_time = 0;

    while (true) {
        uint32_t now = millis();
        if (now - last_log_time >= 1000) {
            last_log_time = now;

            // Calcular frecuencia del ZC
            float freq_hz = g_data.zc_period_us > 0
                ? 1000000.0f / g_data.zc_period_us
                : 0.0f;

            // Log unificado
            Serial.printf(
                "[LOG] PID: temp=%.1f setpoint=%.1f power=%.1f%% | ZC: count=%u period=%u us freq=%.1f Hz running=%d\n",
                g_data.temperature,
                g_data.setpoint,
                g_data.power_percent,
                g_data.zc_count,
                g_data.zc_period_us,
                freq_hz,
                g_data.running
            );
            Serial.flush();
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}