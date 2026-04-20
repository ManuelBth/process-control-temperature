// ============================================================================
// SENSOR CONFIG - PT100 sensor and filtering configuration
// ============================================================================
// Project: Thermal Plant Temperature Control
// Microcontroller: Arduino Nano ESP32
// ============================================================================

#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

#include <stdint.h>

// ============================================================================
// PT100 SENSOR CONFIGURATION
// ============================================================================

// PT100 pins (defined in pins.h)
// #define PT100_ADC_PIN ...

// PT100 voltage divider configuration
#define PT100_R_PULLUP      1000    // Pull-up resistor (ohms)
#define PT100_R_REF         100     // PT100 reference at 0°C (ohms)

// PT100 temperature range
#define PT100_TEMP_MIN      -400    // -4°C (×100)
#define PT100_TEMP_MAX      15000   // 150°C (×100)

// PT100 ADC configuration
#define PT100_ADC_RESOLUTION  12    // 12-bit ADC (0-4095)
#define PT100_ADC_VREF        3300  // ADC Vref (mV)

// PT100 conversion parameters
#define PT100_COEFF_A    3.9083e-3  // Callendar-Van Dusen coefficient A
#define PT100_COEFF_B    -5.775e-7  // Callendar-Van Dusen coefficient B

// ============================================================================
// FILTERING CONFIGURATION
// ============================================================================

// Moving average filter window size
#define SENSOR_FILTER_WINDOW    8

// Exponential filter alpha (0.0-1.0, lower = more smoothing)
#define SENSOR_FILTER_ALPHA      0.2f

// ============================================================================
// SENSOR TIMING
// ============================================================================

// Sensor read interval (ms) - 50ms = 20Hz
#define SENSOR_READ_INTERVAL_MS  50
#define SENSOR_READ_RATE_HZ      20

// Sensor validation timeout (ms)
#define SENSOR_TIMEOUT_MS        1000

// ============================================================================
// SENSOR LIMITS
// ============================================================================

// Valid temperature range
#define SENSOR_TEMP_MIN         -400   // -4°C (×100)
#define SENSOR_TEMP_MAX         15000  // 150°C (×100)

// Rate of change limit (°C/sec × 100)
#define SENSOR_MAX_RATE_CHANGE  500

// ADC raw value limits
#define SENSOR_ADC_MIN    100    // Minimum valid ADC
#define SENSOR_ADC_MAX   4000   // Maximum valid ADC

#endif // SENSOR_CONFIG_H