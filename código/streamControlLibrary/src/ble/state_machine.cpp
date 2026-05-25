#include "state_machine.h"
#include <Arduino.h>

// ============================================
// Global State Variable
// ============================================
volatile system_state_t g_system_state = STATE_IDLE;
volatile uint32_t g_sample_sequence = 0;
volatile uint32_t g_start_time_ms = 0;

// ============================================
// Initialize State Machine
// ============================================
void state_machine_init(void) {
    g_system_state = STATE_IDLE;
    g_sample_sequence = 0;
    g_start_time_ms = 0;
}

// ============================================
// Get Current State
// ============================================
system_state_t state_machine_get_current_state(void) {
    return g_system_state;
}

// ============================================
// Get Start Time (for timestamp reset)
// ============================================
uint32_t state_machine_get_start_time(void) {
    return g_start_time_ms;
}

// ============================================
// Transition to RUNNING State
// ============================================
void state_machine_transition_to_running(void) {
    if (g_system_state == STATE_RUNNING) {
        return;
    }

    g_system_state = STATE_RUNNING;
    g_sample_sequence = 0;
    g_start_time_ms = millis();

    Serial.println("[SM] State: IDLE → RUNNING");
}

// ============================================
// Transition to IDLE State
// ============================================
void state_machine_transition_to_idle(void) {
    if (g_system_state == STATE_IDLE) {
        return;
    }

    g_system_state = STATE_IDLE;

    Serial.println("[SM] State: RUNNING → IDLE");
}