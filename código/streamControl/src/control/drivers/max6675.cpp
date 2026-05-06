#include <Arduino.h>
#include "control/drivers/max6675.h"
#include "common/pins.h"

void max6675_init() {
    pinMode(PIN_MAX6675_SCK, OUTPUT);
    pinMode(PIN_MAX6675_CS, OUTPUT);
    pinMode(PIN_MAX6675_MISO, INPUT);
    digitalWrite(PIN_MAX6675_SCK, LOW);
    digitalWrite(PIN_MAX6675_CS, HIGH);
}

float max6675_read() {
    uint16_t data = 0;
    digitalWrite(PIN_MAX6675_CS, LOW);
    delayMicroseconds(1);
    for (int i = 15; i >= 0; i--) {
        digitalWrite(PIN_MAX6675_SCK, HIGH);
        delayMicroseconds(1);
        if (digitalRead(PIN_MAX6675_MISO)) data |= (1 << i);
        digitalWrite(PIN_MAX6675_SCK, LOW);
        delayMicroseconds(1);
    }
    digitalWrite(PIN_MAX6675_CS, HIGH);
    if (data & 0x0001) return -100.0f;
    if (data & 0x0004) return -100.0f;
    return ((data >> 3) & 0x0FFF) * 0.25f;
}