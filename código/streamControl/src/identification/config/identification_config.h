// ============================================================================
// IDENTIFICATION CONFIG - System identification configuration
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef IDENTIFICATION_CONFIG_H
#define IDENTIFICATION_CONFIG_H

#include <stdint.h>

// ============================================================================
// PRBS CONFIGURATION
// ============================================================================

// PRBS parameters
#define PRBS_ORDER          7       // PRBS order (7 = 2^7-1 = 127 bits)
#define PRBS_MAX_AMPLITUDE  30       // Max amplitude (%)
#define PRBS_MIN_AMPLITUDE  10       // Min amplitude (%)

// PRBS timing
#define PRBS_BIT_DURATION_MS 500     // Bit duration (ms)
#define PRBS_SETTLE_TIME_MS 2000     // Settle time before PRBS (ms)

// PRBS sequence length
#define PRBS_SEQUENCE_LENGTH 127

// ============================================================================
// STEP RESPONSE CONFIGURATION
// ============================================================================

// Step response parameters
#define STEP_INITIAL_POWER  20       // Initial power (%)
#define STEP_FINAL_POWER   50       // Final step power (%)
#define STEP_AMPLITUDE     30       // Step amplitude (%)

// Step timing
#define STEP_DURATION_MS   30000    // Step duration (ms)
#define STEP_SETTLE_TIME_MS 5000     // Settle time before step (ms)

// ============================================================================
// IDENTIFICATION TIMING
// ============================================================================

// Identification sample rate (ms)
#define IDENT_SAMPLE_INTERVAL_MS 100

// Identification timeout (ms)
#define IDENT_TIMEOUT_MS 90000

// ============================================================================
// MODEL PARAMETERS
// ============================================================================

// Expected model order
#define MODEL_ORDER       1       // First order system
#define MODEL_DEAD_TIME 200     // Dead time (ms) - estimated

// Model limits
#define MODEL_GAIN_MIN  0.5f    // Min gain
#define MODEL_GAIN_MAX  5.0f     // Max gain
#define MODEL_TAU_MIN  1000.0f  // Min time constant (ms)
#define MODEL_TAU_MAX  60000.0f // Max time constant (ms)

// ============================================================================
// MINIMUM SQUARES FITTING
// ============================================================================

#define LSQ_ITERATIONS   100     // Max iterations
#define LSQ_CONVERGENCE  0.01f   // Convergence criterion

#endif // IDENTIFICATION_CONFIG_H