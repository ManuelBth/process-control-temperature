#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "control/drivers/max6675.h"
#include "control/drivers/triac.h"
#include "control/drivers/zero_cross.h"
#include "control/algorithms/pid.h"
#include "control/task/control_task.h"
#include "common/data.h"

extern ControlData g_data;

static uint32_t s_last_control_time = 0;

void control_task_init() {
    max6675_init();
    triac_init();
    zc_init();
    pid_init(5.0f, 0.5f, 0.2f);
}

void ControlTask(void* parameter) {
    (void)parameter;

    control_task_init();

    Serial.println("ControlTask started on Core 0");
    Serial.flush();

    s_last_control_time = millis();

    while (true) {
        if (zc_detected()) {
            uint32_t delay_us = map((int)g_data.power_percent, 0, 100, 10000, 100);
            triac_fire((uint16_t)delay_us);
        }

        uint32_t now = millis();
        if (now - s_last_control_time >= 200) {
            s_last_control_time = now;

            float temp = max6675_read();
            g_data.sensor_valid = (temp > -50.0f);
            if (!g_data.sensor_valid) {
                temp = 25.0f;
            }
            g_data.temperature = temp;

            if (g_data.pid_active && g_data.running) {
                float power = pid_calculate(temp, g_data.setpoint, 0.2f);
                g_data.power_percent = power;
                g_data.triac_delay_us = (uint16_t)map((int)power, 0, 100, 10000, 100);
            } else {
                g_data.power_percent = 0.0f;
                g_data.triac_delay_us = 10000;
            }

            Serial.printf("t%lu|%.1f|%.1f|%d\n", now/1000, temp, g_data.setpoint, (int)g_data.power_percent);
            Serial.flush();
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}