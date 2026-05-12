#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "control/drivers/triac.h"
#include "control/drivers/zero_cross.h"
#include "control/task/zc_triac_task.h"

ZcTriacData g_zc_triac_data = {
    .zc_count = 0,
    .period_us = 0,
    .power_percent = 50,
    .running = false
};

void zc_triac_task_init() {
    triac_init();
    zc_init();
    g_zc_triac_data.running = true;
    g_zc_triac_data.power_percent = 50;
    Serial.println("ZcTriacTask: drivers initialized");
}

void ZcTriacTask(void* parameter) {
    (void)parameter;

    zc_triac_task_init();

    Serial.println("ZcTriacTask started on Core 0");
    Serial.flush();

    uint32_t last_print_time = 0;
    uint32_t last_isr_count = 0;

    while (true) {
        // Debug: ver si la ISR se está ejecutando (cada 2 segundos)
        uint32_t now = millis();
        if (now - last_print_time >= 2000) {
            last_print_time = now;

            // Debug ISR count
            uint32_t current_isr = zc_get_isr_count();
            Serial.printf("[ZC-DEBUG] isr_count=%u (delta=%d in 2s)\n",
                         current_isr, current_isr - last_isr_count);
            last_isr_count = current_isr;

            // Datos del ZC-TRIAC
            float freq_hz = g_zc_triac_data.period_us > 0
                ? 1000000.0f / g_zc_triac_data.period_us
                : 0.0f;
            Serial.printf("[ZC-TRIAC] count=%u period=%u us (%.1f Hz) power=%u%% delay=%u us\n",
                         g_zc_triac_data.zc_count,
                         g_zc_triac_data.period_us,
                         freq_hz,
                         g_zc_triac_data.power_percent,
                         g_zc_triac_data.fire_delay_us);
            Serial.flush();
        }

        // Esperar a que ocurra un cruce por cero
        if (zc_detected()) {
            g_zc_triac_data.zc_count++;
            g_zc_triac_data.period_us = zc_get_period_us();

            // Armar el timer del TRIAC con el semiciclo actual y power
            triac_arm(g_zc_triac_data.period_us, g_zc_triac_data.power_percent);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}