// ============================================================================
// MANUAL CONTROL IMPLEMENTATION - Con serial compacta
// ============================================================================

#include "manual_control.h"
#include "../sensors/data/sensor_data.h"
#include "../actuation/data/actuation_data.h"
#include <Arduino.h>

// ============================================================================
// STATE
// ============================================================================

static bool s_active = false;
static uint8_t s_angle = 0;
static uint32_t s_start_time = 0;
static float s_setpoint = 0.0f;

// ============================================================================
// PRIVATE FUNCTIONS
// ============================================================================

static void process_serial(void) {
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.length() == 0) return;

    char first = cmd.charAt(0);

    switch (first) {
        case 'm': {
            // Manual angle: m50 -> 50%
            int angle = cmd.substring(1).toInt();
            if (angle > 100) angle = 100;
            s_angle = (uint8_t)angle;
            Serial.printf("m%d\n", angle);
            break;
        }

        case 's': {
            // Setpoint: s85 -> 85°C
            float sp = cmd.substring(1).toFloat();
            s_setpoint = sp;
            Serial.printf("s%.1f\n", sp);
            break;
        }

        case '1': {
            // ON
            s_active = true;
            s_start_time = millis();
            Serial.println("ON");
            break;
        }

        case '0': {
            // OFF
            s_active = false;
            g_triac_pwr = 0;
            g_act_en = false;
            Serial.println("OFF");
            break;
        }

        case '?': {
            // Status: ?angle,setpoint,active
            Serial.printf("?%d,%.1f,%d\n", s_angle, s_setpoint, s_active ? 1 : 0);
            break;
        }

        default:
            break;
    }
}

// ============================================================================
// PUBLIC FUNCTIONS
// ============================================================================

void manual_init(void) {
    s_active = false;
    s_angle = 0;
    s_start_time = 0;
    s_setpoint = 0.0f;
}

void manual_set_angle(uint8_t angle) {
    if (angle > 100) angle = 100;
    s_angle = angle;
}

bool manual_is_active(void) {
    return s_active;
}

void manual_enable(bool enable) {
    s_active = enable;
    if (enable) {
        s_start_time = millis();
    }
}

float manual_get_setpoint(void) {
    return s_setpoint;
}

void manual_loop(void) {
    // Procesar comandos serial
    process_serial();

    if (!s_active) return;

    // Aplicar ángulo al TRIAC
    g_triac_pwr = s_angle;
    g_act_en = true;

    // Calcular tiempo transcurrido
    uint32_t elapsed = millis() - s_start_time;

    // Imprimir estado cada 500ms
    static uint32_t last_print = 0;
    if (elapsed - last_print >= 500) {
        last_print = elapsed;
        Serial.printf("t%lu|%.2f|%.1f|%d\n", elapsed / 1000, g_temp, s_setpoint, s_angle);
    }
}