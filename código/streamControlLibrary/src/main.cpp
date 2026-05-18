#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ble/ble_handler.h"
#include "ble/state_machine.h"
#include "ble/fake_data.h"
#include "ble/message_parser.h"

// ============================================
// Global Sample Interval (from message_parser)
// ============================================
extern volatile uint32_t g_sample_interval_ms;

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
            Serial.printf("[TX] Sample %d/%d\n", i + 1, g_buffer_count);
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
    Serial.println("  StreamControl ESP32 - BLE Test");
    Serial.println("========================================");
    Serial.println("  Build: " __DATE__ " " __TIME__);
    Serial.println("========================================");
    Serial.println();
    Serial.println("Ready - Waiting for app connection");
    Serial.println();
}

// ============================================
// Arduino Setup
// ============================================
void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial
    }

    print_banner();

    // Initialize components
    state_machine_init();
    fake_data_init();
    ble_init_deferred();

    g_last_sample_time = millis();
    g_last_batch_time = millis();
}

// ============================================
// Arduino Main Loop
// ============================================
void loop() {
    uint32_t current_time = millis();

    system_state_t current_state = state_machine_get_current_state();

    if (current_state == STATE_RUNNING) {
        uint32_t sample_interval = (g_sample_interval_ms > 0) ? g_sample_interval_ms : 100;

        // Collect samples
        if (current_time - g_last_sample_time >= sample_interval) {
            if (g_buffer_count < MAX_SAMPLES_PER_BATCH) {
                g_sample_buffer[g_buffer_count] = generate_sample(current_time);
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

    delay(1);

    // Restart advertising if client disconnected
    ble_restart_advertising_from_loop();
}