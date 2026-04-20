// ============================================================================
// DISPLAY DATA - Display domain state and data structures
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef DISPLAY_DATA_H
#define DISPLAY_DATA_H

#include <stdint.h>
#include <stdbool.h>

#include "display_config.h"

// ============================================================================
// DISPLAY STATE STRUCTURES
// ============================================================================

// Display buffer
typedef struct {
    // Text buffers (2 rows × 16 cols)
    char line[LCD_ROWS][LCD_COLS + 1];

    // Cursor position
    uint8_t cursor_row;
    uint8_t cursor_col;

    // Update flags
    bool update_pending;
    bool line_dirty[LCD_ROWS];
} display_buffer_t;

// Display state
typedef struct {
    // Initialization
    bool initialized;
    bool init_success;

    // Screen management
    display_screen_t current_screen;
    display_screen_t next_screen;

    // Buffer
    display_buffer_t buffer;

    // Timing
    millis_t last_update_ms;
    millis_t last_backlight_ms;

    // Brightness (0-255)
    uint8_t brightness;

    // Statistics
    uint32_t update_count;
    uint32_t error_count;
} display_state_t;

// ============================================================================
// GLOBAL STATE
// ============================================================================

extern display_state_t g_display_state;

// ============================================================================
// ACCESS MACROS
// ============================================================================

#define g_display_init    g_display_state.initialized
#define g_display_ok    g_display_state.init_success
#define g_display_screen g_display_state.current_screen
#define g_display_update g_display_state.buffer.update_pending
#define g_display_bright  g_display_state.brightness

// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================

void display_data_init(void);

#endif // DISPLAY_DATA_H