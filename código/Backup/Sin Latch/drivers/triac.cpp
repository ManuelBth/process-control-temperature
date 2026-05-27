// ============================================================================
// triac.cpp — Control de TRIAC por ángulo de fase con ESP32 Hardware Timer
// ============================================================================

#include <Arduino.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include "control/drivers/triac.h"
#include "common/pins.h"

// Duración del pulso al gate del optotriac en microsegundos.
#define TRIAC_PULSE_US  100UL

// Handles para los dos timers: uno para el disparo (gate ON)
// y otro para el apagado del pulso (gate OFF).
static esp_timer_handle_t s_timer_fire = NULL;
static esp_timer_handle_t s_timer_off  = NULL;

// ─── ISR del timer de APAGADO ────────────────────────────────────────────────
static void IRAM_ATTR triac_off_cb(void* arg) {
    GPIO.out_w1tc = (1UL << PIN_TRIAC);
}

// ─── ISR del timer de DISPARO ────────────────────────────────────────────────
static void IRAM_ATTR triac_fire_cb(void* arg) {
    GPIO.out_w1ts = (1UL << PIN_TRIAC);
    esp_timer_start_once(s_timer_off, TRIAC_PULSE_US);
}

// ─── Inicialización ──────────────────────────────────────────────────────────
void triac_init() {
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << PIN_TRIAC);
    cfg.mode         = GPIO_MODE_OUTPUT;
    cfg.pull_up_en   = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type    = GPIO_INTR_DISABLE;
    gpio_config(&cfg);

    GPIO.out_w1tc = (1UL << PIN_TRIAC);

    esp_timer_create_args_t args_fire = {};
    args_fire.callback = &triac_fire_cb;
    args_fire.name     = "triac_fire";
    esp_timer_create(&args_fire, &s_timer_fire);

    esp_timer_create_args_t args_off = {};
    args_off.callback = &triac_off_cb;
    args_off.name     = "triac_off";
    esp_timer_create(&args_off, &s_timer_off);

    Serial.printf("TRIAC init on pin %d | Pulse width: %lu us\n",
                  PIN_TRIAC, TRIAC_PULSE_US);
}

// ─── Armar el disparo del TRIAC ──────────────────────────────────────────────
void IRAM_ATTR triac_arm(uint32_t semiciclo_us, uint8_t power_percent) {
    esp_timer_stop(s_timer_fire);
    esp_timer_stop(s_timer_off);

    if (power_percent == 0) {
        GPIO.out_w1tc = (1UL << PIN_TRIAC);
        return;
    }

    if (power_percent >= 100) {
        GPIO.out_w1ts = (1UL << PIN_TRIAC);
        esp_timer_start_once(s_timer_off, TRIAC_PULSE_US);
        return;
    }

    // Ángulo de fase: delay = semiciclo × (1 - power / 100)
    uint32_t delay_us = (uint32_t)semiciclo_us * (100U - (uint32_t)power_percent) / 100U;
    esp_timer_start_once(s_timer_fire, delay_us);
}