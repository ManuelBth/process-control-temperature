// ============================================================================
// IDENTIFICATION DATA - Identification domain state and data structures
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef IDENTIFICATION_DATA_H
#define IDENTIFICATION_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "identification_config.h"

// ============================================================================
// IDENTIFICATION STATE STRUCTURES
// ============================================================================

// Identification type
typedef enum {
    IDENT_TYPE_NONE = 0,
    IDENT_TYPE_STEP,
    IDENT_TYPE_PRBS
} identification_type_t;

// Identification status
typedef enum {
    IDENT_STATUS_IDLE = 0,
    IDENT_STATUS_SETTLING,
    IDENT_STATUS_RUNNING,
    IDENT_STATUS_COMPLETE,
    IDENT_STATUS_ERROR
} identification_status_t;

// Identification state
typedef struct {
    // Status
    identification_type_t type;
    identification_status_t status;

    // Excitation level
    percent_t excitation_level;

    // Model parameters (first-order with dead time)
    // G(s) = K * exp(-theta*s) / (tau*s + 1)
    float model_gain;       // K - static gain
    float model_tau;       // tau - time constant (ms)
    float model_theta;     // theta - dead time (ms)
    bool model_valid;

    // Data buffers for identification
    temperature_raw_t y_data[500];   // Output data
    percent_t u_data[500];           // Input data
    uint16_t data_count;
    uint16_t max_samples;

    // Statistics
    millis_t start_time_ms;
    millis_t last_sample_ms;
    float fit_error;
    float r_squared;
} identification_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern identification_state_t g_identification_state;

// ============================================================================
// ACCESS MACROS
// ============================================================================

#define g_ident_type    g_identification_state.type
#define g_ident_status g_identification_state.status
#define g_ident_active (g_identification_state.status == IDENT_STATUS_RUNNING || \
                        g_identification_state.status == IDENT_STATUS_SETTLING)
#define g_ident_model_v g_identification_state.model_valid
#define g_ident_gain   g_identification_state.model_gain
#define g_ident_tau    g_identification_state.model_tau
#define g_ident_theta  g_identification_state.model_theta

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void identification_data_init(void);

#endif // IDENTIFICATION_DATA_H