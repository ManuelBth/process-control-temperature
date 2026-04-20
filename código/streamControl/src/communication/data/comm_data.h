// ============================================================================
// COMMUNICATION DATA - Communication domain state and data structures
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef COMM_DATA_H
#define COMM_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "comm_config.h"

// ============================================================================
// COMMUNICATION STATE STRUCTURES
// ============================================================================

// Communication state
typedef struct {
    // Connection status
    bool serial_connected;
    bool link_active;

    // RX buffer
    char rx_buffer[COMM_RX_BUFFER_SIZE];
    uint16_t rx_index;
    bool rx_overflow;

    // TX buffer
    char tx_buffer[COMM_TX_BUFFER_SIZE];
    uint16_t tx_index;
    bool tx_busy;

    // Command buffer
    char cmd_buffer[COMM_CMD_BUFFER_SIZE];
    char cmd_args[COMM_MAX_ARGS][16];
    uint8_t cmd_argc;

    // Timing
    millis_t last_rx_ms;
    millis_t last_tx_ms;
    millis_t last_heartbeat_ms;

    // Statistics
    uint32_t rx_bytes;
    uint32_t tx_bytes;
    uint32_t rx_errors;
    uint32_t tx_errors;
    uint32_t cmd_count;
} comm_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern comm_state_t g_comm_state;

// ============================================================================
// ACCESS MACROS
// ============================================================================

#define g_comm_connected   g_comm_state.serial_connected
#define g_comm_link       g_comm_state.link_active
#define g_comm_last_rx   g_comm_state.last_rx_ms
#define g_comm_rx_bytes   g_comm_state.rx_bytes
#define g_comm_tx_bytes  g_comm_state.tx_bytes
#define g_comm_cmd_cnt  g_comm_state.cmd_count

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void comm_data_init(void);

#endif // COMM_DATA_H