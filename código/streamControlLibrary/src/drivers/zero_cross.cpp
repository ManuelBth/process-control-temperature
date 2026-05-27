// ============================================================================
// zero_cross.cpp — Detector de cruce por cero para MOC3021 + BTA08
//
// Flujo por semiciclo:
//   1. Flanco RISING en PIN_ZERO_CROSS → entra zc_isr()
//   2. ISR baja PIN_TRIAC a LOW  (apaga gate → TRIAC deja de conducir)
//   3. ISR lee power_percent atómicamente
//   4. ISR llama triac_arm() → programa esp_timer con el delay calculado
//   5. Timer expira → sube PIN_TRIAC a HIGH (TRIAC empieza a conducir)
//   6. TRIAC conduce con señal continua hasta el próximo ZC (paso 2)
// ============================================================================

#include <Arduino.h>
#include <driver/gpio.h>
#include "control/drivers/zero_cross.h"
#include "control/drivers/triac.h"
#include "common/pins.h"
#include "common/pins.h"

// ─── Variables compartidas ISR ↔ tarea ──────────────────────────────────────
volatile bool     g_zc_event     = false;
volatile uint32_t g_zc_period_us = 0;
volatile uint32_t g_zc_last_us   = 0;
volatile uint32_t g_zc_isr_count = 0;
volatile uint8_t  g_power_percent = 0;

// ─── Rango válido de semiciclos ──────────────────────────────────────────────
// 50Hz → semiciclo = 10000 µs
// 60Hz → semiciclo =  8333 µs
// Márgenes amplios para tolerar variaciones de red
#define ZC_PERIOD_MIN_US   7000UL
#define ZC_PERIOD_MAX_US  11000UL

// ─── ISR del cruce por cero ──────────────────────────────────────────────────
void IRAM_ATTR zc_isr() {
    uint32_t now = micros();
    g_zc_isr_count++;

    // ── PASO 1: Apagar gate INMEDIATAMENTE ───────────────────────────────────
    // Esta es la primera acción, sin condiciones. En cada ZC el gate va a LOW.
    // Esto garantiza que el BTA08 deja de conducir al inicio de cada semiciclo.
    GPIO.out_w1tc = (1UL << PIN_TRIAC);
    // ─────────────────────────────────────────────────────────────────────────

    if (g_zc_last_us > 0) {
        uint32_t period = now - g_zc_last_us;

        if (period >= ZC_PERIOD_MIN_US && period <= ZC_PERIOD_MAX_US) {
            g_zc_period_us = period;

            // Señalar evento para telemetría de la tarea
            __atomic_store_n((bool*)&g_zc_event, true, __ATOMIC_RELEASE);

            // ── PASO 2: Armar el timer de disparo ────────────────────────────
            // Lee power atómicamente y programa el delay para subir el gate
            uint8_t pwr = __atomic_load_n((uint8_t*)&g_power_percent, __ATOMIC_ACQUIRE);
            triac_arm(period, pwr);
            // ─────────────────────────────────────────────────────────────────
        }
    }

    g_zc_last_us = now;
}

// ─── Inicialización ──────────────────────────────────────────────────────────
void zc_init() {
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << PIN_ZERO_CROSS);
    cfg.mode         = GPIO_MODE_INPUT;
    cfg.pull_up_en   = GPIO_PULLUP_ENABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type    = GPIO_INTR_POSEDGE;
    gpio_config(&cfg);

    attachInterrupt(digitalPinToInterrupt(PIN_ZERO_CROSS), zc_isr, RISING);

    Serial.printf("ZC init on pin %d | semiciclo esperado: %lu–%lu µs\n",
                  PIN_ZERO_CROSS, ZC_PERIOD_MIN_US, ZC_PERIOD_MAX_US);
}

// ─── API pública ─────────────────────────────────────────────────────────────

bool zc_detected() {
    bool expected = true;
    return __atomic_compare_exchange_n(
        (bool*)&g_zc_event,
        &expected,
        false,
        false,
        __ATOMIC_ACQ_REL,
        __ATOMIC_RELAXED
    );
}

void zc_set_power(uint8_t power_percent) {
    __atomic_store_n((uint8_t*)&g_power_percent, power_percent, __ATOMIC_RELEASE);
}

uint32_t zc_get_period_us() {
    return __atomic_load_n((uint32_t*)&g_zc_period_us, __ATOMIC_ACQUIRE);
}

uint32_t zc_get_isr_count() {
    return __atomic_load_n((uint32_t*)&g_zc_isr_count, __ATOMIC_ACQUIRE);
}