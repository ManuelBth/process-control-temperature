// ============================================================================
// DISPLAY CONFIG - LCD 16x2 display configuration
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <stdint.h>

// ============================================================================
// LCD 16x2 CONFIGURATION
// ============================================================================

// LCD dimensions
#define LCD_COLS  16
#define LCD_ROWS   2

// LCD address (I2C)
#define LCD_I2C_ADDRESS  0x27

// LCD I2C pins (defined in pins.h)
// #define LCD_SDA_PIN ...
// #define LCD_SCL_PIN ...

// ============================================================================
// LCD TIMING
// ============================================================================

// I2C clock frequency (Hz)
#define LCD_I2C_FREQ  100000

// LCD initialization delay (ms)
#define LCD_INIT_DELAY_MS  50

// LCD command delay (us)
#define LCD_COMMAND_DELAY_US  50

// LCD data enable delay (us)
#define LCD_ENABLE_DELAY_US    1

// ============================================================================
// DISPLAY TIMING
// ============================================================================

// Display update interval (ms)
#define DISPLAY_UPDATE_INTERVAL_MS 500

// Display refresh rate (ms)
#define DISPLAY_REFRESH_RATE_MS  250

// ============================================================================
// DISPLAY FORMAT
// ============================================================================

// Temperature format
#define DISPLAY_TEMP_FORMAT  "%d.%02dC"

// Percentage format
#define DISPLAY_PERCENT_FORMAT "%d%%"

// Time format
#define DISPLAY_TIME_FORMAT "%02d:%02d"

// ============================================================================
// CURSOR POSITIONS
// ============================================================================

// Line 0: Status line
#define DISPLAY_LINE_STATUS  0

// Line 1: Temperature line
#define DISPLAY_LINE_TEMP  1

// Row 1 custom positions
#define DISPLAY_COL_TEMP  0    // "Temp: XX.XXC"
#define DISPLAY_COL_SETP  9    // "Set: XXC"
#define DISPLAY_COL_MODE 14    // "M" (mode indicator)

// ============================================================================
// SCREEN CONFIGURATION
// ============================================================================

// Number of screens
#define DISPLAY_SCREEN_COUNT  3

// Screen IDs
typedef enum {
    SCREEN_HOME = 0,
    SCREEN_STATUS,
    SCREEN_CONFIG
} display_screen_t;

// Default screen
#define DISPLAY_DEFAULT_SCREEN  SCREEN_HOME

#endif // DISPLAY_CONFIG_H