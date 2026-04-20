// ============================================================================
// StreamControl - Control de Proceso de Temperatura
// Firmware based on FreeRTOS for ESP32
// ============================================================================

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Forward declarations
extern void task_manager_init(void);

// ============================================================================
// setup() - Runs once on power-up or reset
// ============================================================================
void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    delay(100);

    Serial.println("\n\n=== StreamControl Starting ===");

    // Initialize FreeRTOS task manager
    task_manager_init();

    Serial.println("=== System Ready ===");
}

// ============================================================================
// loop() - Main application loop (should be empty in FreeRTOS)
// ============================================================================
void loop() {
    // Empty - all work done by FreeRTOS tasks
    vTaskDelay(pdMS_TO_TICKS(portMAX_DELAY));
}