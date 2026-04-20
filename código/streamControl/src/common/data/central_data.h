// ============================================================================
// CENTRAL DATA - Única fuente de verdad del sistema
// ============================================================================
// Proyecto: Control de Temperatura Planta Térmica
// Microcontrolador: Arduino Nano ESP32
// ============================================================================

#ifndef CENTRAL_DATA_H
#define CENTRAL_DATA_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// SECCIÓN: TIPOS BASE DEL SISTEMA
// ============================================================================

// Temperatura: -40.00°C a 150.00°C → multiplicado por 100
// Rango: -4000 a 15000 (int16_t es suficiente)
typedef int16_t temperature_raw_t;
#define TEMP_RAW_MIN      -4000
#define TEMP_RAW_MAX      15000
#define TEMP_MIN_SETPOINT 1800    // 18°C mínimo
#define TEMP_MAX_SETPOINT 15000   // 150°C máximo
#define TEMP_TO_RAW(x)    ((temperature_raw_t)((x) * 100))
#define RAW_TO_TEMP(x)    ((float)(x) / 100.0f)

// Porcentaje: 0-100%
typedef uint8_t percent_t;
#define PERCENT_MIN  0
#define PERCENT_MAX  100

// Timestamp Unix (32 bits hasta 2038)
typedef uint32_t timestamp_t;

// Millis overflow safe
typedef uint32_t millis_t;

// ============================================================================
// SECCIÓN: FLAGS DEL SISTEMA (Bit Field)
// ============================================================================

typedef union {
    uint8_t raw;
    struct {
        uint8_t system_enabled    : 1;  // Sistema habilitado
        uint8_t overtemp_error   : 1;  // Error sobre temperatura
        uint8_t sensor_error     : 1;  // Error de sensor
        uint8_t identification_active : 1;  // Identificación en proceso
        uint8_t emergency_stop   : 1;  // Parada de emergencia
        uint8_t config_loaded    : 1;  // Configuración cargada
        uint8_t reserved1        : 1;  // Reservado
        uint8_t reserved2        : 1;  // Reservado
    } bit;
} system_flags_t;

// ============================================================================
// SECCIÓN: MODOS DE CONTROL
// ============================================================================

typedef enum {
    CONTROL_MODE_OFF = 0,      // Sistema apagado
    CONTROL_MODE_MANUAL,       // Control manual
    CONTROL_MODE_PID,          // Control PID
    CONTROL_MODE_RST           // Control RST
} control_mode_t;

// ============================================================================
// SECCIÓN: DATOS DE SENSORES (sensor)
// ============================================================================

typedef struct {
    temperature_raw_t temperature;    // Temperatura actual × 100
    temperature_raw_t temperature_filtered;  // Temperatura filtrada × 100
    uint16_t adc_raw;               // Lectura ADC cruda
    bool valid;                      // Dato válido
    millis_t last_read_ms;           // Timestamp última lectura
} sensor_data_t;

// ============================================================================
// SECCIÓN: DATOS DE CONTROL (control)
// ============================================================================

typedef struct {
    // Modo de operación
    control_mode_t mode;                    // Modo actual
    control_mode_t previous_mode;           // Modo anterior (para recuperación)

    // Setpoint
    temperature_raw_t setpoint;              // Setpoint × 100

    // Salida de control (0-100%)
    percent_t control_output;               // Salida calculada

    // Parámetros PID
    float kp;                                // Ganancia proporcional
    float ki;                                // Ganancia integral
    float kd;                                // Ganancia derivativa

    // Estado PID interno
    float pid_integral;                      // Término integral
    temperature_raw_t pid_last_error;        // Error anterior
    bool pid_first_run;                      // Primera ejecución

    // Parámetros RST (polinomios)
    float r_coeffs[3];                       // Coeficientes R
    float s_coeffs[3];                       // Coeficientes S
    float t_coeffs[3];                       // Coeficientes T

    // Historial para RST
    temperature_raw_t error_history[3];       // Errores anteriores
    percent_t output_history[3];             // Salidas anteriores

    // Tuning
    bool auto_tune;                          // Auto-tuning activo
} control_data_t;

// ============================================================================
// SECCIÓN: DATOS DE ACTUADORES (actuation)
// ============================================================================

typedef struct {
    // TRIAC (resistencia)
    percent_t triac_power;          // Potencia TRIAC 0-100%
    bool triac_enabled;             // Habilitado

    // Ventilador 1 (PWM)
    percent_t fan1_speed;           // Velocidad 0-100%
    bool fan1_enabled;              // Habilitado

    // Ventilador 2 (ON/OFF)
    bool fan2_on;                   // Estado on/off

    // Relay principal
    bool relay_on;                  // Relay principal

    // Estados de seguridad
    bool safety_override;           // Override de seguridad activo
} actuation_data_t;

// ============================================================================
// SECCIÓN: DATOS DE IDENTIFICACIÓN (identification)
// ============================================================================

typedef enum {
    IDENT_NONE = 0,                 // Sin identificación
    IDENT_STEP_RESPONSE,            // Respuesta escalón
    IDENT_PRBS                      // Señal PRBS
} identification_type_t;

typedef enum {
    IDENT_IDLE = 0,                 // Sin actividad
    IDENT_RUNNING,                  // En proceso
    IDENT_COMPLETE,                 // Completado
    IDENT_ERROR                     // Error
} identification_status_t;

typedef struct {
    identification_type_t type;              // Tipo de identificación
    identification_status_t status;        // Estado
    percent_t excitation_level;            // Nivel de excitación (0-100%)

    // Modelo del sistema (primer orden con retardo)
    // G(s) = K * e^(-theta*s) / (tau*s + 1)
    float model_gain;              // K - ganancia estática
    float model_tau;               // tau - constante de tiempo
    float model_theta;             // theta - retardo

    // Datos para identificación
    uint16_t sample_count;         // Contador de muestras
    uint16_t max_samples;          // Máximo de muestras
    millis_t start_time_ms;        // Tiempo de inicio

    // Resultados
    bool model_valid;              // Modelo válido
    float fit_error;              // Error de ajuste
} identification_data_t;

// ============================================================================
// SECCIÓN: DATOS DE DISPLAY (display)
// ============================================================================

typedef struct {
    bool initialized;              // Display inicializado
    uint8_t brightness;           // Brillo (0-255)
    millis_t last_update_ms;       // Última actualización
    uint16_t refresh_rate_ms;     // Rate de refresco
} display_data_t;

// ============================================================================
// SECCIÓN: DATOS DE COMUNICACIÓN (comm)
// ============================================================================

typedef struct {
    // Serial
    bool serial_connected;        // Serial disponible

    // Buffer de comandos
    char command_buffer[64];      // Buffer de comandos
    uint8_t buffer_index;         // Índice actual

    // Estado
    millis_t last_comm_ms;        // Última comunicación
} comm_data_t;

// ============================================================================
// ESTRUCTURA CENTRALIZADA PRINCIPAL
// ============================================================================

typedef struct {
    // Secciones del sistema
    sensor_data_t       sensor;         // Datos de sensores
    control_data_t     control;        // Datos de control
    actuation_data_t  actuation;       // Datos de actuadores
    identification_data_t identification;  // Datos de identificación
    display_data_t     display;        // Datos del display
    comm_data_t        comm;           // Datos de comunicación

    // Flags globales
    system_flags_t      flags;          // Flags del sistema
} central_data_t;

// Instancia global - única fuente de verdad del sistema
extern central_data_t g_system_data;

// ============================================================================
// MACROS DE ACCESO RÁPIDO
// ============================================================================

// Sensores
#define g_temp          g_system_data.sensor.temperature
#define g_temp_filtered g_system_data.sensor.temperature_filtered
#define g_temp_valid    g_system_data.sensor.valid

// Control
#define g_mode          g_system_data.control.mode
#define g_setpoint      g_system_data.control.setpoint
#define g_control_output g_system_data.control.control_output

// Actuation
#define g_triac_power   g_system_data.actuation.triac_power
#define g_fan1_speed    g_system_data.actuation.fan1_speed
#define g_fan2_on       g_system_data.actuation.fan2_on
#define g_relay_on      g_system_data.actuation.relay_on

// Flags
#define g_system_enabled   g_system_data.flags.bit.system_enabled
#define g_overtemp_error  g_system_data.flags.bit.overtemp_error
#define g_sensor_error    g_system_data.flags.bit.sensor_error

// Display
#define g_display_init    g_system_data.display.initialized

// ============================================================================
// FUNCIONES DE INICIALIZACIÓN
// ============================================================================

void central_data_init(void);

#endif // CENTRAL_DATA_H
