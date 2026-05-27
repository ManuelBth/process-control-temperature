// ============================================================================
// StreamControl - Unified ESP32 Entry Point
// Combines BLE initialization (main1.cpp) + FreeRTOS tasks (main2.cpp)
// ============================================================================

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ble/ble_handler.h"
#include "ble/state_machine.h"
#include "ble/message_parser.h"
#include "ble/fake_data.h"
#include "common/data.h"
#include "control/task/control_task.h"
#include "control/task/zc_triac_task.h"
#include "control/task/logger_task.h"
#include "drivers/fan.h"

// create_batch_json returns char* - declared in fake_data.h
extern char* create_batch_json(sample_t* samples, uint16_t count);

// ============================================
// Generate REAL Sample from g_data
// ============================================
static sample_t generate_real_sample(uint32_t timestamp) {
    sample_t sample;
    sample.t = timestamp;
    sample.temp = g_data.temperature;     // Temperatura real del MAX6675
    sample.angle = g_data.power_percent;  // Power percent (TRIAC)
    sample.pwm = g_data.fan_speed;        // PWM ventiladores
    return sample;
}

// ============================================
// Sample Buffer for Batching
// ============================================
static sample_t g_sample_buffer[MAX_SAMPLES_PER_BATCH];
static uint16_t g_buffer_count = 0;

// ============================================
// Timing Variables
// ============================================
static uint32_t g_last_sample_time = 0;
static uint32_t g_last_batch_time = 0;
static uint32_t g_batch_interval_ms = 1000;  // 1 second batch interval

// ============================================
// Send Batch and Reset Buffer
// ============================================
static void send_batch(void) {
    if (g_buffer_count == 0) {
        return;
    }

    for (uint16_t i = 0; i < g_buffer_count; i++) {
        char* json_str = create_batch_json(&g_sample_buffer[i], 1);
        if (json_str != nullptr) {
            ble_send_notification((const uint8_t*)json_str, strlen(json_str));
            free(json_str);
        }
        delay(2);  // 2ms delay between notifies
    }

    g_buffer_count = 0;
}

// ============================================
// Startup Banner
// ============================================
static void print_banner(void) {
    Serial.println();
    Serial.println("========================================");
    Serial.println("  StreamControl ESP32 - Unified Build");
    Serial.println("========================================");
    Serial.println("  Build: " __DATE__ " " __TIME__);
    Serial.println("========================================");
    Serial.println();
    Serial.println("Ready - Waiting for app connection");
    Serial.println();
}

// ============================================
// Arduino Setup - Combines BLE + FreeRTOS Init
// ============================================
void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial
    }

    print_banner();

    // Initialize fan PWM — arranca directo con velocidad base (50%)
    fan_init();

    // Initialize BLE subsystem (creates NimBLE task internally)
    state_machine_init();
    ble_init_deferred();

    g_last_sample_time = millis();
    g_last_batch_time = millis();

    // Create FreeRTOS tasks
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

// ============================================
// Arduino Main Loop - Sample Batching + BLE
// ============================================
void loop() {
    uint32_t current_time = millis();

    system_state_t current_state = state_machine_get_current_state();

    if (current_state == STATE_RUNNING) {
        uint32_t sample_interval = (g_sample_interval_ms > 0) ? g_sample_interval_ms : 100;

        // Collect samples with relative timestamp (from start_time)
        if (current_time - g_last_sample_time >= sample_interval) {
            if (g_buffer_count < MAX_SAMPLES_PER_BATCH) {
                uint32_t start_time = state_machine_get_start_time();
                uint32_t relative_time = current_time - start_time;
                g_sample_buffer[g_buffer_count] = generate_real_sample(relative_time);
                g_buffer_count++;
            }
            g_last_sample_time = current_time;
        }

        // Send batch every second
        if (current_time - g_last_batch_time >= g_batch_interval_ms) {
            send_batch();
            g_last_batch_time = current_time;
        }
    } else {
        // If state changed to IDLE, flush any remaining samples
        if (g_buffer_count > 0) {
            send_batch();
        }
    }

    // FreeRTOS delay
    vTaskDelay(10 / portTICK_PERIOD_MS);

    // Restart advertising if client disconnected
    ble_restart_advertising_from_loop();
}