// ============================================================================
// ZcTriacTask - Zero Cross detection and TRIAC control
// ============================================================================

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "control/drivers/triac.h"
#include "control/drivers/zero_cross.h"
#include "control/task/zc_triac_task.h"
#include "common/data.h"

extern ControlData g_data;

void zc_triac_task_init() {
    triac_init();
    zc_init();
    Serial.println("ZcTriacTask: TRIAC and ZC initialized");
}

void ZcTriacTask(void* parameter) {
    (void)parameter;

    zc_triac_task_init();

    Serial.println("ZcTriacTask started");
    Serial.flush();

    while (true) {
        // Esperar a que ocurra un cruce por cero
        if (zc_detected()) {
            g_data.zc_count++;
            g_data.zc_period_us = zc_get_period_us();

            // Usar power_percent calculado por el PID
            triac_arm(g_data.zc_period_us, (uint16_t)g_data.power_percent);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}