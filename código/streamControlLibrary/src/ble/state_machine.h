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
// Global State Variable
// ============================================
extern volatile system_state_t g_system_state;

// ============================================
// Sample Sequence Counter (for control_data messages)
// ============================================
extern volatile uint32_t g_sample_sequence;

// ============================================
// State Machine Function Declarations
// ============================================

// Initialize state machine to IDLE
void state_machine_init(void);

// Get current system state
system_state_t state_machine_get_current_state(void);

// Transition from IDLE to RUNNING
// - Logs state change to Serial
// - Resets sample sequence counter
void state_machine_transition_to_running(void);

// Transition from RUNNING to IDLE
// - Logs state change to Serial
// - Stops sending control_data
void state_machine_transition_to_idle(void);

#endif // STATE_MACHINE_H