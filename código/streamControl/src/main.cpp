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

// ============================================================================
// setup() - Runs once on power-up or reset
// ============================================================================
void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    delay(100);

    Serial.println("\n\n=== StreamControl Starting ===");

    // Initialize manual control
    manual_init();

    // Initialize FreeRTOS task manager
    task_manager_init();

    Serial.println("=== System Ready ===");
}

// ============================================================================
// loop() - Main application loop (should be empty in FreeRTOS)
// ============================================================================
void loop() {
    // Manual control loop (serial + control)
    manual_loop();

    // Small delay to not hog CPU
    vTaskDelay(pdMS_TO_TICKS(10));
}