// ============================================================================
// StreamControl - FreeRTOS Integration
// ============================================================================

#include <Arduino.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "common/pins.h"
#include "common/data.h"
#include "control/drivers/max6675.h"
#include "control/drivers/triac.h"
#include "control/drivers/zero_cross.h"
#include "control/algorithms/pid.h"
#include "control/task/control_task.h"

extern ControlData g_data;

void zc_print_status() {
    Serial.printf("ZC: period=%u us, last=%u us, event=%d\n",
                 zc_get_period_us(),
                 g_zc_last_us,
                 g_zc_event ? 1 : 0);
    Serial.flush();
}

void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== StreamControl FreeRTOS ===");
    Serial.flush();

    max6675_init();
    triac_init();
    zc_init();

    g_data.setpoint = 50.0f;
    g_data.running = true;
    g_data.pid_active = true;

    BaseType_t ret = xTaskCreatePinnedToCore(
        ControlTask,
        "ControlTask",
        4096,
        nullptr,
        5,
        nullptr,
        0
    );

    if (ret == pdPASS) {
        Serial.println("ControlTask created on Core 0");
    } else {
        Serial.println("ERROR: Failed to create ControlTask");
    }

    Serial.println("=== Ready ===");
    Serial.flush();
}

void loop() {
    if (Serial.available()) {
        char c = Serial.read();
        if (c == 's' && Serial.available()) {
            g_data.setpoint = Serial.parseFloat();
            Serial.printf("Setpoint: %.1f\n", g_data.setpoint);
        }
        if (c == 'p') {
            g_data.pid_active = true;
            g_data.running = true;
            pid_reset();
            Serial.println("ON");
        }
        if (c == 'o') {
            g_data.pid_active = false;
            g_data.running = false;
            Serial.println("OFF");
        }
        if (c == 'z') {
            zc_print_status();
        }
        Serial.flush();
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
}