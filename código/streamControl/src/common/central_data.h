// ============================================================================
// CENTRAL DATA - Datos esenciales del sistema
// ============================================================================

#ifndef CENTRAL_DATA_H
#define CENTRAL_DATA_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// TIPOS
// ============================================================================

typedef uint8_t percent_t;  // 0-100%

typedef enum {
    CONTROL_OFF = 0,
    CONTROL_MANUAL,
    CONTROL_PID,
    CONTROL_RST
} control_mode_t;

// ============================================================================
// CONTROL
// ============================================================================

typedef struct {
    control_mode_t mode;
    float setpoint;      // Temperatura objetivo (°C)
    float temperature;   // Temperatura actual (°C)
    float kp;           // PID: ganancia proporcional
    float ki;           // PID: ganancia integral
    float kd;           // PID: ganancia derivativa
    float output;       // Salida del controlador (0-100%)
} control_data_t;

// ============================================================================
// ACTUADORES
// ============================================================================

typedef struct {
    percent_t triac_power;    // Potencia TRIAC (0-100%)
    bool triac_enabled;
    
    percent_t fan1_speed;    // Ventilador 1 PWM (0-100%)
    bool fan1_enabled;
    
    bool fan2_on;           // Ventilador 2 ON/OFF (perturbaciones)
    bool relay_on;          // Relay principal
} actuator_data_t;

// ============================================================================
// DATOS GLOBALES
// ============================================================================

extern control_data_t g_control;
extern actuator_data_t g_actuator;

#endif
