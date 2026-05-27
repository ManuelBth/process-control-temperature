// ============================================================================
// zc_triac_task.cpp — Tarea FreeRTOS de supervisión ZC + TRIAC
//
// Esta tarea NO controla el TRIAC directamente.
// El control ocurre en la ISR del ZC con precisión de microsegundos.
//
// Responsabilidades de esta tarea:
//   1. Leer power_percent del PID y actualizarlo atómicamente para la ISR
//   2. Telemetría: contar ZC y registrar período
//   3. Watchdog: apagar TRIAC si se pierde la señal ZC
// ============================================================================

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include "control/drivers/zero_cross.h"
#include "control/drivers/triac.h"
#include "common/data.h"

extern ControlData g_data;

static SemaphoreHandle_t s_data_mutex  = NULL;
static uint32_t          s_last_zc_tick = 0;
static bool              s_zc_lost      = false;

#define ZC_WATCHDOG_MS  200UL

// ─── Inicialización ──────────────────────────────────────────────────────────
void zc_triac_task_init() {
    triac_init();
    zc_init();
    s_data_mutex   = xSemaphoreCreateMutex();
    s_last_zc_tick = xTaskGetTickCount();
    Serial.println("ZcTriacTask: inicializado (MOC3021 + BTA08 sin latch)");
}

// ─── Tarea principal ─────────────────────────────────────────────────────────
void ZcTriacTask(void* parameter) {
    (void)parameter;
    zc_triac_task_init();

    while (true) {
        // ── 1. Actualizar power_percent para la ISR ───────────────────────────
        if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(2)) == pdTRUE) {
            float raw = g_data.power_percent;
            if (raw < 0.0f)   raw = 0.0f;
            if (raw > 100.0f) raw = 100.0f;
            zc_set_power((uint8_t)raw);
            xSemaphoreGive(s_data_mutex);
        }

        // ── 2. Telemetría ─────────────────────────────────────────────────────
        if (zc_detected()) {
            s_last_zc_tick = xTaskGetTickCount();
            s_zc_lost = false;

            if (xSemaphoreTake(s_data_mutex, pdMS_TO_TICKS(1)) == pdTRUE) {
                g_data.zc_count++;
                g_data.zc_period_us = zc_get_period_us();
                xSemaphoreGive(s_data_mutex);
            }
        }

        // ── 3. Watchdog ───────────────────────────────────────────────────────
        uint32_t elapsed = (xTaskGetTickCount() - s_last_zc_tick) * portTICK_PERIOD_MS;
        if (elapsed > ZC_WATCHDOG_MS && !s_zc_lost) {
            s_zc_lost = true;
            zc_set_power(0);
            triac_arm(0, 0);
            Serial.printf("ZcTriacTask: WARNING señal ZC perdida (%lu ms) → TRIAC OFF\n",
                          elapsed);
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}