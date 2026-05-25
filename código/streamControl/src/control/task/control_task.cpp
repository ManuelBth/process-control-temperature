// ============================================================================
// ControlTask - Temperature control with PID
// ============================================================================

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "control/drivers/max6675.h"
#include "control/algorithms/pid.h"
#include "control/task/control_task.h"
#include "common/data.h"

extern ControlData g_data;

static uint32_t s_last_control_time = 0;

void control_task_init() {
    max6675_init();
    pid_init(g_data.kp, g_data.ki, g_data.kd);
    Serial.println("ControlTask: MAX6675 and PID initialized");
}

void ControlTask(void* parameter) {
    (void)parameter;

    control_task_init();

    Serial.println("ControlTask started");
    Serial.flush();

    s_last_control_time = millis();

    while (true) {
        // Control cada 200ms
        uint32_t now = millis();
        if (now - s_last_control_time >= 200) {
            s_last_control_time = now;

            // Leer temperatura
            float temp = max6675_read();
            g_data.sensor_valid = (temp > -50.0f);
            if (!g_data.sensor_valid) {
                temp = 25.0f;
            }
            g_data.temperature = temp;

            // Calcular PID si está activo
            if (g_data.pid_active && g_data.running) {
                float power = pid_calculate(temp, g_data.setpoint, 0.2f);
                g_data.power_percent = power;
            } else {
                g_data.power_percent = 0.0f;
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}