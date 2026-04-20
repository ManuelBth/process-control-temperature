// ============================================================================
// COMMUNICATION CONFIG - Serial communication configuration
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef COMM_CONFIG_H
#define COMM_CONFIG_H

#include <stdint.h>

// ============================================================================
// SERIAL CONFIGURATION
// ============================================================================

// Default baud rate
#define COMM_BAUDRATE_DEFAULT 115200

// Alternative baud rates
#define COMM_BAUDRATE_LOW    9600
#define COMM_BAUDRATE_MID   57600
#define COMM_BAUDRATE_HIGH 230400

// Serial configuration
#define COMM_SERIAL_CONFIG   SERIAL_8N1

// ============================================================================
// BUFFER CONFIGURATION
// ============================================================================

// RX buffer size
#define COMM_RX_BUFFER_SIZE 256

// TX buffer size
#define COMM_TX_BUFFER_SIZE 256

// Command buffer size
#define COMM_CMD_BUFFER_SIZE 64

// Max command arguments
#define COMM_MAX_ARGS 8

// ============================================================================
// TIMING CONFIGURATION
// ============================================================================

// Command timeout (ms)
#define COMM_COMMAND_TIMEOUT_MS 5000

// Character timeout (ms)
#define COMM_CHAR_TIMEOUT_MS 100

// Heartbeat interval (ms)
#define COMM_HEARTBEAT_INTERVAL_MS 1000

// Keep-alive timeout (ms)
#define COMM_KEEPALIVE_TIMEOUT_MS 30000

// ============================================================================
// PROTOCOL CONFIGURATION
// ============================================================================

// Command delimiter
#define COMM_DELIMITER '\n'

// Argument separator
#define COMM_ARG_SEPARATOR ','

// Echo enable
#define COMM_ECHO_ENABLED 1

// ============================================================================
// SERIAL PINS (defined in pins.h)
// ============================================================================

// #define COMM_TX_PIN ...
// #define COMM_RX_PIN ...

#endif // COMM_CONFIG_H