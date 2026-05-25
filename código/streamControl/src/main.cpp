// ============================================================================
// StreamControl - FreeRTOS Integration
// ============================================================================

#include <Arduino.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "common/data.h"
#include "control/task/control_task.h"
#include "control/task/zc_triac_task.h"
#include "control/task/logger_task.h"

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== StreamControl FreeRTOS ===");
    Serial.flush();

    // Crear ControlTask (lee sensor, calcula PID)
    BaseType_t ret1 = xTaskCreatePinnedToCore(
        ControlTask,
        "ControlTask",
        4096,
        nullptr,
        5,
        nullptr,
        0  // Core 0
    );
    Serial.println(ret1 == pdPASS ? "ControlTask created on Core 0" : "ERROR: ControlTask");

    // Crear ZcTriacTask (detecta ZC, dispara TRIAC)
    BaseType_t ret2 = xTaskCreatePinnedToCore(
        ZcTriacTask,
        "ZcTriacTask",
        4096,
        nullptr,
        5,
        nullptr,
        1  // Core 1
    );
    Serial.println(ret2 == pdPASS ? "ZcTriacTask created on Core 1" : "ERROR: ZcTriacTask");

    // Crear LoggerTask (log cada 1 segundo)
    BaseType_t ret3 = xTaskCreatePinnedToCore(
        LoggerTask,
        "LoggerTask",
        4096,
        nullptr,
        3,
        nullptr,
        0  // Core 0
    );
    Serial.println(ret3 == pdPASS ? "LoggerTask created on Core 0" : "ERROR: LoggerTask");

    Serial.println("=== Ready ===");
    Serial.flush();
}

void loop() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}