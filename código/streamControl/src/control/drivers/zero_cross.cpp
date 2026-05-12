#include <Arduino.h>
#include "control/drivers/zero_cross.h"
#include "common/pins.h"

volatile bool g_zc_event = false;
volatile uint32_t g_zc_period_us = 0;
volatile uint32_t g_zc_last_us = 0;
volatile uint32_t g_zc_isr_count = 0;
volatile uint8_t g_zc_last_state = 0;

void IRAM_ATTR zc_isr() {
    uint32_t now = micros();
    g_zc_isr_count++;

    uint8_t current_state = digitalRead(PIN_ZERO_CROSS);

    if (current_state != g_zc_last_state) {
        g_zc_last_state = current_state;

        if (g_zc_last_us > 0) {
            uint32_t period = now - g_zc_last_us;
            // Solo aceptar si es una señal real de AC (>5ms = <200Hz)
            if (period > 5000) {
                g_zc_period_us = period;
                g_zc_event = true;
            }
        }
        g_zc_last_us = now;
    }
}

void zc_init() {
    pinMode(PIN_ZERO_CROSS, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ZERO_CROSS), zc_isr, RISING);
    Serial.printf("ZC init on pin %d (RISING)\n", PIN_ZERO_CROSS);
}

bool zc_detected() {
    if (g_zc_event) {
        g_zc_event = false;
        return true;
    }
    return false;
}

uint32_t zc_get_period_us() {
    return g_zc_period_us;
}

uint32_t zc_get_isr_count() {
    return g_zc_isr_count;
}