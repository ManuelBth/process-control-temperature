// ============================================================================
// SENSOR DATA IMPLEMENTATION
// ============================================================================

#include "sensor_data.h"

// ============================================================================
// GLOBAL STATE
// ============================================================================

sensor_state_t g_sensor_state = {
    .temperature = 25.0f,
    .mode = SENSOR_MODE_NORMAL,
    .interval_ms = SENSOR_INTERVAL_NORMAL_MS,
    .enabled = false,
    .error = false,
    .last_read_ms = 0
};

// ============================================================================
// FUNCTIONS
// ============================================================================

void sensor_data_init(void) {
    g_sensor_state.temperature = 25.0f;
    g_sensor_state.mode = SENSOR_MODE_NORMAL;
    g_sensor_state.interval_ms = SENSOR_INTERVAL_NORMAL_MS;
    g_sensor_state.enabled = false;
    g_sensor_state.error = false;
    g_sensor_state.last_read_ms = 0;
}