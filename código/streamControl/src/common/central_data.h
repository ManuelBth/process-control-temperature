// ============================================================================
// CENTRAL DATA - Single source of truth for the system
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef COMMON_DATA_CENTRAL_DATA_H
#define COMMON_DATA_CENTRAL_DATA_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// OPTIMIZED TYPES
// ============================================================================

// Temperature: -40.00°C to 150.00°C -> int16_t (×100 for decimal)
#define TEMP_MIN  -4000
#define TEMP_MAX  15000

typedef int16_t temperature_raw_t;
#define TEMP_TO_RAW(x) ((temperature_raw_t)((x) * 100))
#define RAW_TO_TEMP(x) ((float)(x) / 100.0f)

// Percentage: 0-100% -> uint8_t
typedef uint8_t percent_t;

// ============================================================================
// CONTROL MODES
// ============================================================================

typedef enum {
    CONTROL_OFF = 0,
    CONTROL_MANUAL,
    CONTROL_PID,
    CONTROL_RST
} control_mode_t;

// ============================================================================
// SYSTEM FLAGS
// ============================================================================

typedef union {
    uint8_t raw;
    struct {
        uint8_t system_enabled : 1;
        uint8_t overtemp_error : 1;
        uint8_t sensor_error  : 1;
        uint8_t ident_active : 1;
        uint8_t emergency_stop : 1;
        uint8_t reserved    : 3;
    } bit;
} system_flags_t;

// ============================================================================
// SENSOR DATA
// ============================================================================

typedef struct {
    temperature_raw_t temperature;
    uint32_t last_read_time;
    bool valid_reading;
} sensor_data_t;

// ============================================================================
// CONTROL DATA
// ============================================================================

typedef struct {
    control_mode_t mode;
    temperature_raw_t setpoint;
    temperature_raw_t output;
    float integral;
    temperature_raw_t prev_error;
    float kp, ki, kd;
} control_data_t;

// ============================================================================
// ACTUATOR DATA
// ============================================================================

typedef struct {
    percent_t triac_power;
    percent_t fan1_speed;
    bool fan2_on;
    bool relay_on;
    bool triac_active;
} actuator_data_t;

// ============================================================================
// IDENTIFICATION DATA
// ============================================================================

typedef struct {
    bool active;
    uint8_t state;
    uint8_t prbs_amplitude;
    uint32_t sample_count;
} identification_data_t;

// ============================================================================
// DISPLAY DATA
// ============================================================================

typedef struct {
    bool initialized;
    uint8_t brightness;
} display_data_t;

// ============================================================================
// COMMUNICATION DATA
// ============================================================================

typedef struct {
    bool connected;
    uint32_t last_tx_time;
} comm_data_t;

// ============================================================================
// CENTRAL DATA STRUCTURE
// ============================================================================

typedef struct {
    sensor_data_t sensor;
    control_data_t control;
    actuator_data_t actuator;
    identification_data_t identification;
    display_data_t display;
    comm_data_t comm;
    system_flags_t flags;
} central_data_t;

// Global instance - single source of truth
extern central_data_t g_system_data;

// Quick access macros
#define g_temp          g_system_data.sensor.temperature
#define g_setpoint      g_system_data.control.setpoint
#define g_control_mode g_system_data.control.mode
#define g_triac_power  g_system_data.actuator.triac_power
#define g_fan1_speed  g_system_data.actuator.fan1_speed
#define g_fan2_on     g_system_data.actuator.fan2_on
#define g_relay_on     g_system_data.actuator.relay_on
#define g_system_on   g_system_data.flags.bit.system_enabled

#endif // COMMON_DATA_CENTRAL_DATA_H