// ============================================================================
// StreamControl - Control de Proceso de Temperatura
// Firmware based on FreeRTOS for ESP32
// ============================================================================

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Forward declarations
extern void task_manager_init(void);
extern void manual_init(void);
extern void manual_loop(void);

// Debug macro que usa Serial0 (Hardware UART) como fallback
// El Arduino Nano ESP32 tiene Serial0 en D0/D1 (GPIO43/GPIO44)
#if defined(ARDUINO_USB_CDC_ON_BOOT)
    #define DEBUG Serial
#else
    #define DEBUG Serial0
#endif

// ============================================================================
// setup() - Runs once on power-up or reset
// ============================================================================
void setup() {
    // Initialize USB Serial FIRST
    Serial.begin(115200);
    
    // Wait a bit for both serials
    delay(500);
    
    Serial.println("\n\n=== StreamControl Starting ===");
    Serial.flush();

    Serial.println("Serial initialized");
    Serial.flush();

    // Initialize manual control
    manual_init();
    Serial.println("[Init] Manual control done");
    Serial.flush();

    // Initialize FreeRTOS task manager
    task_manager_init();
    
    Serial.println("=== System Ready ===");
    Serial.flush();
}

// ============================================================================
// loop() - Main application loop (should be empty in FreeRTOS)
// ============================================================================
void loop() {
    // Manual control loop (serial + control)
    manual_loop();

    // Small delay to not hog CPU
    delay(10);
}