#include <Arduino.h>
#include "control/drivers/zero_cross.h"
#include "common/pins.h"

volatile bool g_zc_event = false;
volatile uint32_t g_zc_period_us = 20000;
volatile uint32_t g_zc_last_us = 0;

void IRAM_ATTR zc_isr() {
    uint32_t now = micros();
    if (g_zc_last_us > 0) {
        g_zc_period_us = now - g_zc_last_us;
    }
    g_zc_last_us = now;
    g_zc_event = true;
}

void zc_init() {
    pinMode(PIN_ZERO_CROSS, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_ZERO_CROSS), zc_isr, CHANGE);
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