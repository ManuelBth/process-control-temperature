// ============================================================================
// PINS - Pin definitions for ESP32 DevKit V1
// ============================================================================
//
// ESP32 DevKit V1 Pinout:
// -------------------------------------------------------------------------
// | Pin  | GPIO | Notes                                      |
// |------|------|-------------------------------------------|
// | D0   | GPIO0 | Boot pin, LED on some boards              |
// | D1   | GPIO1 |                                         |
// | D2   | GPIO2 | LED (on most DevKit boards)              |
// | D3   | GPIO3 |                                         |
// | D4   | GPIO4 |                                         |
// | D5   | GPIO5 | SPI Flash (usually not available)          |
// | D6   | GPIO6 | SPI Flash (not available)                |
// | D7   | GPIO7 | SPI Flash (not available)                |
// | D8   | GPIO8 | SPI Flash (not available)                |
// | D9   | GPIO9 | SPI Flash (not available)                |
// | D10  | GPIO10| SPI Flash (not available)                |
// | D11  | GPIO11| SPI Flash (not available)                |
// | D12  | GPIO12| JTAG (also Boot pin if pulled low)        |
// | D13  | GPIO13|                                         |
// | D14  | GPIO14|                                         |
// | D15  | GPIO15|                                         |
// | D16  | GPIO16| UART0                                  |
// | D17  | GPIO17| UART0                                  |
// | D18  | GPIO18| SPI VSCLK                              |
// | D19  | GPIO19| SPI VMISO                             |
// | D21  | GPIO21| I2C SDA                               |
// | D22  | GPIO22| I2C SCL                               |
// | D23  | GPIO23| SPI VMOSI                             |
// | D25  | GPIO25| DAC1                                  |
// | D26  | GPIO26| DAC2                                  |
// | D27  | GPIO27|                                         |
// | D32  | GPIO32|                                         |
// | D33  | GPIO33|                                         |
// | D34  | GPIO34| Input only (ADC)                       |
// | D35  | GPIO35| Input only (ADC)                       |
// | D36  | GPIO36| Input only (ADC)                       |
// | D39  | GPIO39| Input only (ADC)                       |
// -------------------------------------------------------------------------
// Reference: https://lastminuteengineers.com/esp32-pinout-reference/
// ============================================================================

#ifndef PINS_H
#define PINS_H

#include <cstdint>

// ============================================================================
// TRIAC
// ============================================================================
// GPIO25 - DAC capable, good for PWM with low-pass filter

constexpr int PIN_TRIAC = 25;            // Output: TRIAC trigger (GPIO25)

// ============================================================================
// ZERO CROSS
// ============================================================================
// GPIO4 - Interrupt capable, safe GPIO

constexpr int PIN_ZERO_CROSS = 4;         // Input: zero-cross detector (GPIO4)

// ============================================================================
// FANS
// ============================================================================
// GPIO26 - PWM capable
// GPIO27 - PWM capable, safe GPIO

constexpr int PIN_FAN1_PWM = 26;        // Output: PWM fan 1 (GPIO26)
constexpr int PIN_FAN2 = 27;            // Output: ON/OFF fan 2 (GPIO27)

// ============================================================================
// MAX6675 THERMOCOUPLE (SPI)
// ============================================================================
// Pines SPI para MAX6675 según Random Nerd Tutorials
// VSPI default pins on ESP32 DevKit

constexpr int PIN_MAX6675_SCK = 5;      // Clock (GPIO5)
constexpr int PIN_MAX6675_MISO = 19;    // Data output (GPIO19)
constexpr int PIN_MAX6675_CS = 23;      // Chip select (GPIO23)

// ============================================================================
// RELAY
// ============================================================================
// GPIO14 - Safe GPIO

constexpr int PIN_RELAY = 14;            // Output: main relay (GPIO14)

// ============================================================================
// ELECTRICAL CONFIGURATION
// ============================================================================

constexpr int MAINS_FREQ_HZ = 50;      // Mains frequency (Hz)
constexpr int HALF_CYCLE_US = 10000;    // Half-period (10ms @ 50Hz)

#endif