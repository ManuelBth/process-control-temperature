// ============================================================================
// PINS - Implementación de funciones de gestión de pines
// ============================================================================
// Proyecto: Control de Temperatura Planta Térmica
// Microcontrolador: Arduino Nano ESP32
// ============================================================================

#include "pins.h"

// ============================================================================
// INICIALIZACIÓN DE PINES
// ============================================================================

void pins_init(void) {
    // Pines ANALÓGICOS (ADC)
    // ---------------------------
    // PT100 usa ADC1_CH0 (GPIO 36) - configurado automáticamente por analogRead()

    // Pines de SALIDA (OUTPUT)
    // ---------------------------
    pinMode(GPIO_PIN_TRIAC, OUTPUT);
    pinMode(GPIO_PIN_FAN1, OUTPUT);
    pinMode(GPIO_PIN_FAN2, OUTPUT);
    pinMode(GPIO_PIN_RELAY_MAIN, OUTPUT);

    // Pines LED
    // Nota: GPIO 2 tiene LED integrado en algunos boards
    #ifdef LED_BUILTIN
    pinMode(GPIO_PIN_LED_STATUS, OUTPUT);
    #endif
    pinMode(GPIO_PIN_LED_ERROR, OUTPUT);

    // Pines de ENTRADA (INPUT)
    // ---------------------------
    //Detector de cruce por cero
    pinMode(GPIO_PIN_ZERO_CROSS, INPUT);

    // Botones con pull-up interno
    pinMode(GPIO_PIN_BTN_EMERGENCY, INPUT_PULLUP);
    pinMode(GPIO_PIN_BTN_MODE, INPUT_PULLUP);
    pinMode(GPIO_PIN_BTN_UP, INPUT_PULLUP);
    pinMode(GPIO_PIN_BTN_DOWN, INPUT_PULLUP);

    // CONFIGURACIÓN PWM
    // ---------------------------
    // Ventilador 1: 25 kHz PWM
    ledcSetup(0, FAN1_PWM_FREQ, 8);  // Canal 0, 25kHz, 8 bits
    ledcAttachPin(GPIO_PIN_FAN1, 0); // GPIO 18 al canal 0

    // TRIAC: PWM para modulación de fase
    // Frecuencia se ajusta dinámicamente según cruce por cero
    ledcSetup(1, 1000, 8);  // Canal 1, 1kHz (inicial), 8 bits
    ledcAttachPin(GPIO_PIN_TRIAC, 1); // GPIO 16 al canal 1

    // ESTADOS INICIALES DE SEGURIDAD
    // ---------------------------
    digitalWrite(GPIO_PIN_TRIAC, TRIAC_FIRE_OFF);
    digitalWrite(GPIO_PIN_FAN1, FAN1_OFF);
    digitalWrite(GPIO_PIN_FAN2, FAN2_OFF);
    digitalWrite(GPIO_PIN_RELAY_MAIN, RELAY_OFF);

    // LEDs inicializados
    #ifdef LED_BUILTIN
    digitalWrite(GPIO_PIN_LED_STATUS, LED_OFF);
    #endif
    digitalWrite(GPIO_PIN_LED_ERROR, LED_OFF);

    // CONFIGURACIÓN ADC
    // ---------------------------
    analogReadResolution(12);  // 12 bits (0-4095)
    analogSetAttenuation(ADC_11db);  // Rango 0-3.3V
}

// ============================================================================
// VERIFICACIÓN DE PINES (DEBUG)
// ============================================================================

void pins_verify(void) {
    #ifdef DEBUG
    Serial.println(F("=== Verificación de Pines ==="));

    // Verificar pines I2C (deben ser 21 y 22)
    if (GPIO_PIN_I2C_SDA != 21 || GPIO_PIN_I2C_SCL != 22) {
        Serial.println(F("ERROR: Pines I2C deben ser 21 y 22"));
    } else {
        Serial.println(F("OK: Pines I2C correctos (21, 22)"));
    }

    // Verificar pines UART (deben ser 1 y 3)
    if (GPIO_PIN_UART_TX != 1 || GPIO_PIN_UART_RX != 3) {
        Serial.println(F("ERROR: Pines UART deben ser 1 y 3"));
    } else {
        Serial.println(F("OK: Pines UART correctos (1, 3)"));
    }

    // Pines especial: ADC para PT100
    Serial.print(F("PT100 ADC: GPIO "));
    Serial.println(GPIO_PIN_PT100);

    Serial.println(F("=== Verificación completada ==="));
    #endif
}