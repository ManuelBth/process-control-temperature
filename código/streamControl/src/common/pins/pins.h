// ============================================================================
// PINS - Definiciones centralizadas de pines GPIO
// ============================================================================
// Proyecto: Control de Temperatura Planta Térmica
// Microcontrolador: Arduino Nano ESP32
// ============================================================================

#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// ============================================================================
// SECCIÓN: PT100 SENSOR (Termorresistencia)
// ============================================================================
// PT100: 100Ω a 0°C, resistencia que varía con temperatura
// Usando divisor de voltaje con resistencia conocida

// Pino analógico para PT100
constexpr int GPIO_PIN_PT100 = 36;        // GPIO 36 (ADC1_CH0) - Solo input
constexpr int PT100_ADC_CHANNEL = 0;

// Resistencia del divisor (10kΩ)
constexpr float PT100_SERIES_RESISTOR = 10000.0f;

// Resistencia PT100 a 0°C
constexpr float PT100_R0 = 100.0f;

// Coeficiente A de Steinhart-Hart para PT100
constexpr float PT100_A = 3.9083e-3f;
constexpr float PT100_B = -5.775e-7f;

// ============================================================================
// SECCIÓN: TRIAC + DETECTOR CRUCE POR CERO
// ============================================================================
// TRIAC: Control de potencia de resistencia térmica
// Detector de cruce por cero: sincronización a 50Hz

// Pin de salida para TRIAC (disparo)
constexpr int GPIO_PIN_TRIAC = 16;        // GPIO 16 - Salida PWM

// Pin de entrada para detector cruce por cero
constexpr int GPIO_PIN_ZERO_CROSS = 17;   // GPIO 17 - InputInterrupt

// Nivel lógico para activar TRIAC
constexpr int TRIAC_FIRE_ON = HIGH;
constexpr int TRIAC_FIRE_OFF = LOW;

// Tensiones de red
constexpr int MAINS_FREQUENCY_HZ = 50;       // 50 Hz
constexpr int MAINS_PERIOD_MS = 20;           // 20 ms período
constexpr int HALF_PERIOD_MS = 10;           // 10 ms semiperíodo

// Timeout máximo para sincronización
constexpr uint32_t ZERO_CROSS_TIMEOUT_US = 11000;  // 11ms (margen)

// ============================================================================
// SECCIÓN: VENTILADORES
// ============================================================================

// Ventilador 1: Control PWM (0-100%)
constexpr int GPIO_PIN_FAN1 = 18;           // GPIO 18 - Salida PWM
constexpr int FAN1_PWM_FREQ = 25000;         // 25 kHz (frecuencia típica fans)

// Nivel lógico para activar
constexpr int FAN1_ON = HIGH;
constexpr int FAN1_OFF = LOW;

// Ventilador 2: ON/OFF (para perturbaciones)
constexpr int GPIO_PIN_FAN2 = 19;          // GPIO 19 - Salida digital

constexpr int FAN2_ON = HIGH;
constexpr int FAN2_OFF = LOW;

// ============================================================================
// SECCIÓN: RELAY PRINCIPAL
// ============================================================================
// Relay principal: Corte general de potencia a resistencia

constexpr int GPIO_PIN_RELAY_MAIN = 21;       // GPIO 21 - Salida digital

constexpr int RELAY_ON = LOW;               // Activo bajo (típico)
constexpr int RELAY_OFF = HIGH;

// ============================================================================
// SECCIÓN: DISPLAY LCD 16x2 (I2C)
// ============================================================================
// LCD 16x2 con controlador HD44780 vía I2C
// Dirección típica: 0x27 o 0x3F

// Pines I2C (fijos en ESP32)
constexpr int GPIO_PIN_I2C_SDA = 21;        // GPIO 21 - FIJO
constexpr int GPIO_PIN_I2C_SCL = 22;        // GPIO 22 - FIJO

// Dirección I2C del LCD
constexpr uint8_t LCD_I2C_ADDRESS = 0x27;

// Dimensiones LCD
constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;

// ============================================================================
// SECCIÓN: COMUNICACIÓN SERIAL
// ============================================================================
// UART0 para debug/configuración (115200 baud)

// Pines UART0 (fijos)
constexpr int GPIO_PIN_UART_TX = 1;        // GPIO 1 - TX
constexpr int GPIO_PIN_UART_RX = 3;        // GPIO 3 - RX

// Baudrate
constexpr uint32_t SERIAL_BAUD = 115200;

// ============================================================================
// SECCIÓN: LEDS INDICADORES
// ============================================================================

// LED de estado (integrado en muchos boards)
constexpr int GPIO_PIN_LED_STATUS = 2;      // GPIO 2 - LED integrado

constexpr int LED_ON = LOW;
constexpr int LED_OFF = HIGH;

// LED de error
constexpr int GPIO_PIN_LED_ERROR = 4;        // GPIO 4

// ============================================================================
// SECCIÓN: BOTONES (entrada)
// ============================================================================

// Botón de emergencia
constexpr int GPIO_PIN_BTN_EMERGENCY = 35; // GPIO 35 - Solo input

// Botón de modo
constexpr int GPIO_PIN_BTN_MODE = 34;     // GPIO 34 - Solo input

// Estado activo (pull-up interno)
constexpr int BTN_ACTIVE = LOW;
constexpr int BTN_INACTIVE = HIGH;

// ============================================================================
// SECCIÓN: BOTONES CON PULL-EXTERNO
// ============================================================================

// Botón incremento setpoint
constexpr int GPIO_PIN_BTN_UP = 32;         // GPIO 32

// Botón decremento setpoint
constexpr int GPIO_PIN_BTN_DOWN = 33;        // GPIO 33

// ============================================================================
// AGRUPACIONES PARA INICIALIZACIÓN
// ============================================================================

// Grupo: Pines de sensores analógicos
#define SENSOR_PINS { GPIO_PIN_PT100 }

// Grupo: Pines de actuadores digitales/PWM
#define ACTUATOR_PINS { \
    GPIO_PIN_TRIAC, \
    GPIO_PIN_FAN1,  \
    GPIO_PIN_FAN2,  \
    GPIO_PIN_RELAY_MAIN \
}

// Grupo: Pines de display
#define DISPLAY_PINS { \
    GPIO_PIN_I2C_SDA, \
    GPIO_PIN_I2C_SCL \
}

// Grupo: Pines de comunicación
#define COMM_PINS { \
    GPIO_PIN_UART_TX, \
    GPIO_PIN_UART_RX \
}

// Grupo: Pines LED
#define LED_PINS { \
    GPIO_PIN_LED_STATUS, \
    GPIO_PIN_LED_ERROR \
}

// Grupo: Pines botones entrada
#define BUTTON_PINS { \
    GPIO_PIN_BTN_EMERGENCY, \
    GPIO_PIN_BTN_MODE, \
    GPIO_PIN_BTN_UP, \
    GPIO_PIN_BTN_DOWN \
}

// ============================================================================
// ENUM: TIPOS DE PINES (para configuración)
// ============================================================================

enum PinMode_t {
    PIN_MODE_INPUT = 0,
    PIN_MODE_OUTPUT,
    PIN_MODE_INPUT_PULLUP,
    PIN_MODE_INPUT_PULLDOWN,
    PIN_MODE_ANALOG
};

enum PinGroup_t {
    PIN_GROUP_SENSOR = 0,
    PIN_GROUP_ACTUATOR,
    PIN_GROUP_DISPLAY,
    PIN_GROUP_COMM,
    PIN_GROUP_LED,
    PIN_GROUP_BUTTON
};

// ============================================================================
// FUNCIONES DE GESTIÓN DE PINES
// ============================================================================

// Inicialización de todos los pines
void pins_init(void);

// Verificación de pines (para debug)
void pins_verify(void);

#endif // PINS_H