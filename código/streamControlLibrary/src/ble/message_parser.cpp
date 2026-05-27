#include "message_parser.h"
#include "state_machine.h"
#include "common/data.h"
#include "drivers/fan.h"
#include "cJSON.h"
#include <Arduino.h>

extern ControlData g_data;

// ============================================
// Global sample interval
// ============================================
volatile uint32_t g_sample_interval_ms = 100;
volatile bool g_perturbation_triggered = false;

// ============================================
// Initialize the Message Parser
// ============================================
void message_parser_init(void) {
    g_sample_interval_ms = 100;
    g_perturbation_triggered = false;
}

// ============================================
// Check and reset perturbation flag
// ============================================
bool perturbation_was_triggered(void) {
    if (g_perturbation_triggered) {
        g_perturbation_triggered = false;
        return true;
    }
    return false;
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
        // Extract PID parameters from nested "pid" object
        cJSON* pid = cJSON_GetObjectItem(root, "pid");
        if (cJSON_IsObject(pid)) {
            cJSON* kp = cJSON_GetObjectItem(pid, "kp");
            if (cJSON_IsNumber(kp)) {
                result.kp = (float)kp->valuedouble;
                g_data.kp = (float)kp->valuedouble;
            }

            cJSON* ki = cJSON_GetObjectItem(pid, "ki");
            if (cJSON_IsNumber(ki)) {
                result.ki = (float)ki->valuedouble;
                g_data.ki = (float)ki->valuedouble;
            }

            cJSON* kd = cJSON_GetObjectItem(pid, "kd");
            if (cJSON_IsNumber(kd)) {
                result.kd = (float)kd->valuedouble;
                g_data.kd = (float)kd->valuedouble;
            }

            // setpoint inside pid object
            cJSON* setpoint = cJSON_GetObjectItem(pid, "setpoint");
            if (cJSON_IsNumber(setpoint)) {
                result.setpoint = (float)setpoint->valuedouble;
                g_data.setpoint = (float)setpoint->valuedouble;
            }
        }

        result.valid = true;

    } else if (strcmp(result.type, "start_control") == 0) {
        state_machine_transition_to_running();
        g_data.running = true;
        g_data.pid_active = true;
        result.valid = true;

    } else if (strcmp(result.type, "stop_control") == 0) {
        state_machine_transition_to_idle();
        g_data.running = false;
        g_data.pid_active = false;
        // NOTA: El ventilador NO se apaga. Se mantiene en la velocidad actual
        // (normalmente 20%) para preservar flujo de aire de homogeneización.
        result.valid = true;

    } else if (strcmp(result.type, "perturbation") == 0) {
        cJSON* pertur = cJSON_GetObjectItem(root, "pertur");
        if (cJSON_IsBool(pertur)) {
            g_perturbation_triggered = cJSON_IsTrue(pertur);
            fan_set_perturbation(g_perturbation_triggered);
        }
        result.valid = true;
    }

    cJSON_Delete(root);
    return result;
}