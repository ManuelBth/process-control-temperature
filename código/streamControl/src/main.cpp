// ============================================================================
// StreamControl - FreeRTOS Integration
// ============================================================================

#include <Arduino.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "common/pins.h"
#include "control/drivers/triac.h"
#include "control/drivers/zero_cross.h"
#include "control/task/zc_triac_task.h"

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== StreamControl FreeRTOS ===");
    Serial.flush();

    // Inicializar datos del ZC-TRIAC
    g_zc_triac_data.power_percent = 50;  // 50% hardcoded
    g_zc_triac_data.running = true;

    BaseType_t ret = xTaskCreatePinnedToCore(
        ZcTriacTask,
        "ZcTriacTask",
        4096,
        nullptr,
        5,
        nullptr,
        0
    );

    if (ret == pdPASS) {
        Serial.println("ZcTriacTask created on Core 0");
    } else {
        Serial.println("ERROR: Failed to create ZcTriacTask");
    }

    Serial.println("=== Ready ===");
    Serial.flush();
}

void loop() {
    vTaskDelay(10 / portTICK_PERIOD_MS);
}