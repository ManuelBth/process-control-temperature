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

    // BLE event tracking: detect transitions via g_data changes
    static bool prev_running = false;
    static float prev_kp = 0.0f;
    bool ble_event_logged = false;

    while (true) {
        uint32_t now = millis();

        // Check for BLE events by detecting g_data transitions
        // start_control: running changes from false to true
        if (g_data.running && !prev_running) {
            Serial.println("[BLE] Control STARTED");
            ble_event_logged = true;
        }
        // stop_control: running changes from true to false
        else if (!g_data.running && prev_running) {
            Serial.println("[BLE] Control STOPPED");
            ble_event_logged = true;
        }

        // config_sync: kp changes (message_parser updates kp, ki, kd, setpoint)
        // Detect when kp value actually changes from config_sync
        if (g_data.kp != prev_kp && prev_kp != 0.0f) {
            Serial.printf("[BLE] Config updated: kp=%.2f ki=%.2f kd=%.2f setpoint=%.1f\n",
                g_data.kp, g_data.ki, g_data.kd, g_data.setpoint);
            ble_event_logged = true;
        }

        // Update tracking variables
        prev_running = g_data.running;
        prev_kp = g_data.kp;

        // Periodic log every 1 second
        if (now - last_log_time >= 1000) {
            last_log_time = now;

            // Calcular frecuencia del ZC
            float freq_hz = g_data.zc_period_us > 0
                ? 1000000.0f / g_data.zc_period_us
                : 0.0f;

            // Log unificado
            Serial.printf(
                "[LOG] temp=%.1f°C setpoint=%.1f°C power=%.1f%% | ZC: cnt=%u period=%uµs freq=%.1fHz | running=%d pid=%d\n",
                g_data.temperature,
                g_data.setpoint,
                g_data.power_percent,
                g_data.zc_count,
                g_data.zc_period_us,
                freq_hz,
                g_data.running,
                g_data.pid_active
            );
            Serial.flush();
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}