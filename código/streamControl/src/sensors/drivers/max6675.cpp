// ============================================================================
// MAX6675 DRIVER IMPLEMENTATION
// ============================================================================

#include "max6675.h"
#include <Arduino.h>
#include "pins.h"
#include "sensor_config.h"

// Pines definidos en pins.h
#define MAX6675_SCK  PIN_MAX6675_SCK
#define MAX6675_MISO PIN_MAX6675_MISO
#define MAX6675_CS   PIN_MAX6675_CS

// Pines como macros para velocidad
#define SCK_HIGH   digitalWrite(MAX6675_SCK, HIGH)
#define SCK_LOW    digitalWrite(MAX6675_SCK, LOW)
#define CS_LOW     digitalWrite(MAX6675_CS, LOW)
#define CS_HIGH    digitalWrite(MAX6675_CS, HIGH)
#define READ_MISO   digitalRead(MAX6675_MISO)

// ============================================================================
// INIT
// ============================================================================

void max6675_init(void) {
    // Configurar pines como salida/entrada
    pinMode(MAX6675_SCK, OUTPUT);
    pinMode(MAX6675_CS, OUTPUT);
    pinMode(MAX6675_MISO, INPUT);

    // Estado inicial
    digitalWrite(MAX6675_SCK, LOW);
    digitalWrite(MAX6675_CS, HIGH);

    // Pequeño delay para estabilidad
    delayMicroseconds(1);
}

// ============================================================================
// READ
// ============================================================================

float max6675_read(void) {
    uint16_t data = 0;

    // Iniciar lectura
    CS_LOW;

    // Leer 16 bits (MSB first)
    for (int i = 15; i >= 0; i--) {
        SCK_HIGH;
        delayMicroseconds(1);

        if (READ_MISO) {
            data |= (1 << i);
        }

        SCK_LOW;
        delayMicroseconds(1);
    }

    // Fin lectura
    CS_HIGH;

    // Verificar si hay error de thermoucple
    if (data & 0x0001) {  // Bit 0: Open circuit
        return TEMP_ERROR;
    }

    if (data & 0x0004) {  // Bit 2: Fault
        return TEMP_ERROR;
    }

    // Extraer temperatura (bits 14-3)
    uint16_t temp_raw = (data >> 3) & 0x0FFF;

    // Convertir a Celsius (0.25°C por bit)
    float temperature = temp_raw * 0.25f;

    return temperature;
}

// ============================================================================
// FAULT CHECK
// ============================================================================

bool max6675_is_fault(void) {
    // Leer solo el primer byte para verificar fault
    uint8_t data = 0;

    CS_LOW;

    for (int i = 7; i >= 0; i--) {
        SCK_HIGH;
        delayMicroseconds(1);

        if (READ_MISO) {
            data |= (1 << i);
        }

        SCK_LOW;
        delayMicroseconds(1);
    }

    CS_HIGH;

    // Bit 2 (del segundo byte) o bit 6 (del primero) indica fault
    return (data & 0x04) != 0;
}