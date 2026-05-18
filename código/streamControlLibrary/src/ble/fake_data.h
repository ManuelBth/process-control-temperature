#ifndef FAKE_DATA_H
#define FAKE_DATA_H

#include <Arduino.h>

// ============================================
// Max samples per batch (1 second at 100ms interval)
// ============================================
#define MAX_SAMPLES_PER_BATCH  10

// ============================================
// Fake Control Data Sample Structure
// ============================================
typedef struct {
    uint32_t t;      // timestamp (ms counter)
    float temp;      // temperature (24.0-26.0)
    float angle;    // servo angle (43.0-47.0 degrees)
    float pwm;       // PWM duty cycle (75-85)
} sample_t;

// ============================================
// Function Declarations
// ============================================

// Initialize fake data generator (call once at startup)
void fake_data_init(void);

// Generate a single fake sample with timestamp
sample_t generate_sample(uint32_t timestamp);

// Create JSON string for a batch of samples
// Returns nullptr on error, caller must free returned string with free()
char* create_batch_json(sample_t* samples, uint16_t count);

#endif // FAKE_DATA_H