#include <Arduino.h>
#include "control/drivers/triac.h"
#include "common/pins.h"

void triac_init() {
    pinMode(PIN_TRIAC, OUTPUT);
    digitalWrite(PIN_TRIAC, LOW);
}

void triac_arm(uint16_t semiciclo_us, uint16_t power_percent) {
    if (power_percent == 0) {
        digitalWrite(PIN_TRIAC, LOW);
        return;
    }

    // Calcular tiempo que debe estar en HIGH proporcional al power
    uint32_t tiempo_high = (uint32_t)semiciclo_us * power_percent / 100;

    // PIN en HIGH
    digitalWrite(PIN_TRIAC, HIGH);

    // Esperar tiempo_high
    delayMicroseconds(tiempo_high);

    // PIN en LOW
    digitalWrite(PIN_TRIAC, LOW);
}