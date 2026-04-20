// ============================================================================
// SENSOR DATA - Sensor domain state and data structures
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "sensor_config.h"

// ============================================================================
// SENSOR STATE STRUCTURES
// ============================================================================

// Sensor reading with filtering
typedef struct {
    // Raw values
    temperature_raw_t temperature_raw;      // Raw temperature (×100)
    temperature_raw_t temperature_filtered; // Filtered temperature (×100)
    uint16_t adc_raw;                       // Raw ADC value

    // Status
    bool valid;                             // Reading is valid
    bool connected;                         // Sensor is connected
    millis_t last_read_ms;                 // Last read timestamp

    // Filter buffer (moving average)
    temperature_raw_t filter_buffer[SENSOR_FILTER_WINDOW];
    uint8_t filter_index;
    int16_t filter_sum;

    // Statistics
    temperature_raw_t temp_min;
    temperature_raw_t temp_max;
    uint16_t read_count;
} sensor_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern sensor_state_t g_sensor_state;

// ============================================================================
// ACCESS MACROS
// ============================================================================

#define g_sensor_temp       g_sensor_state.temperature_raw
#define g_sensor_temp_filt  g_sensor_state.temperature_filtered
#define g_sensor_adc        g_sensor_state.adc_raw
#define g_sensor_valid      g_sensor_state.valid
#define g_sensor_connected  g_sensor_state.connected

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void sensor_data_init(void);

#endif // SENSOR_DATA_H