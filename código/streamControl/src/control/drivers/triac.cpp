#include <Arduino.h>
#include "control/drivers/triac.h"
#include "common/pins.h"

void triac_init() {
    pinMode(PIN_TRIAC, OUTPUT);
    digitalWrite(PIN_TRIAC, LOW);
}

void triac_fire(uint16_t delay_us) {
    digitalWrite(PIN_TRIAC, HIGH);
    delayMicroseconds(100);
    digitalWrite(PIN_TRIAC, LOW);
}