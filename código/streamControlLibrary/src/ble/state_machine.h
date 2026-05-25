#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

// ============================================
// System State Enum
// ============================================
typedef enum {
    STATE_IDLE = 0,
    STATE_RUNNING
} system_state_t;

// ============================================
// Initialize State Machine
// ============================================
void state_machine_init(void);

// ============================================
// Get Current State
// ============================================
system_state_t state_machine_get_current_state(void);

// ============================================
// Transition to RUNNING State
// ============================================
void state_machine_transition_to_running(void);

// ============================================
// Transition to IDLE State
// ============================================
void state_machine_transition_to_idle(void);

// ============================================
// Get Start Time (for timestamp reset)
// ============================================
uint32_t state_machine_get_start_time(void);

#endif // STATE_MACHINE_H