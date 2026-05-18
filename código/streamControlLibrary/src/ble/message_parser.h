#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <stdint.h>

// ============================================
// Parsed Message Structure
// ============================================
typedef struct {
    const char* type;              // Message type string
    uint32_t sample_interval_ms;   // From config_sync
    float kp;                     // PID proportional gain
    float ki;                     // PID integral gain
    float kd;                     // PID derivative gain
    const char* control_mode;     // Control mode string
    float setpoint;                // Target setpoint
    bool valid;                    // Parsing succeeded
} parsed_message_t;

// ============================================
// Message Parser Function Declarations
// ============================================

// Initialize the message parser
void message_parser_init(void);

// Parse incoming JSON message from BLE
// Returns parsed_message_t with valid=true if successful
parsed_message_t message_parser_parse(char* json, int len);

// Get the current system state (for external use)
extern volatile uint32_t g_sample_interval_ms;

#endif // MESSAGE_PARSER_H