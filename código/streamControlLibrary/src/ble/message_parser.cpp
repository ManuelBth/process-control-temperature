#include "message_parser.h"
#include "state_machine.h"
#include "cJSON.h"
#include <Arduino.h>

// ============================================
// Global sample interval
// ============================================
volatile uint32_t g_sample_interval_ms = 100;

// ============================================
// Initialize the Message Parser
// ============================================
void message_parser_init(void) {
    g_sample_interval_ms = 100;
}

// ============================================
// Parse a JSON message and handle by type
// ============================================
parsed_message_t message_parser_parse(char* json, int len) {
    parsed_message_t result = {0};

    if (json == nullptr || len <= 0) {
        return result;
    }

    // Parse the JSON
    cJSON* root = cJSON_Parse(json);
    if (root == nullptr) {
        return result;
    }

    // Extract message type
    cJSON* type_item = cJSON_GetObjectItem(root, "type");
    if (!cJSON_IsString(type_item) || type_item->valuestring == nullptr) {
        cJSON_Delete(root);
        return result;
    }

    result.type = type_item->valuestring;

    // Handle based on message type
    if (strcmp(result.type, "config_sync") == 0) {
        // Extract sample_interval_ms
        cJSON* sample_interval = cJSON_GetObjectItem(root, "sample_interval_ms");
        if (cJSON_IsNumber(sample_interval)) {
            g_sample_interval_ms = (uint32_t)sample_interval->valueint;
        }

        // Extract PID parameters if present
        cJSON* pid = cJSON_GetObjectItem(root, "pid");
        if (cJSON_IsObject(pid)) {
            cJSON* kp = cJSON_GetObjectItem(pid, "kp");
            if (cJSON_IsNumber(kp)) result.kp = (float)kp->valuedouble;

            cJSON* ki = cJSON_GetObjectItem(pid, "ki");
            if (cJSON_IsNumber(ki)) result.ki = (float)ki->valuedouble;

            cJSON* kd = cJSON_GetObjectItem(pid, "kd");
            if (cJSON_IsNumber(kd)) result.kd = (float)kd->valuedouble;
        }

        cJSON* setpoint = cJSON_GetObjectItem(root, "setpoint");
        if (cJSON_IsNumber(setpoint)) {
            result.setpoint = (float)setpoint->valuedouble;
        }

        result.valid = true;

    } else if (strcmp(result.type, "start_control") == 0) {
        state_machine_transition_to_running();
        result.valid = true;

    } else if (strcmp(result.type, "stop_control") == 0) {
        state_machine_transition_to_idle();
        result.valid = true;
    }

    cJSON_Delete(root);
    return result;
}