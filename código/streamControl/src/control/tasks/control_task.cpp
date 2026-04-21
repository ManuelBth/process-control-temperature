// ============================================================================
// CONTROL TASK - Tarea del controlador
// ============================================================================

#include "control_task.h"
#include "control_data.h"
#include "algorithms/pid.h"
#include "../sensors/data/sensor_data.h"
#include "../sensors/config/sensor_config.h"
#include "../actuation/data/actuation_data.h"

// ============================================================================
// TASK
// ============================================================================

void vControlTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(PID_SAMPLE_TIME_MS);

    for (;;) {
        // Leer temperatura del sensor
        float temp = g_temp;

        // Guardar temperatura en estado
        g_ctrl_temp = temp;

        // Cambiar modo del sensor según control
        if (g_ctrl_en && (g_ctrl_mode == CONTROL_MODE_PID || g_ctrl_mode == CONTROL_MODE_RST)) {
            g_sensor_mode = SENSOR_MODE_PID;
        } else {
            g_sensor_mode = SENSOR_MODE_NORMAL;
        }

        // Verificar modo y calcular
        if (g_ctrl_en) {
            switch (g_ctrl_mode) {
                case CONTROL_MODE_OFF:
                    g_ctrl_out = 0.0f;
                    break;

                case CONTROL_MODE_MANUAL:
                    g_ctrl_out = g_control_state.manual_power;
                    break;

                case CONTROL_MODE_PID: {
                    float sp = g_ctrl_sp;
                    float dt = PID_SAMPLE_TIME_MS / 1000.0f;
                    g_ctrl_out = pid_calculate(temp, sp, dt);
                    break;
                }

                case CONTROL_MODE_RST:
                case CONTROL_MODE_IDENT:
                    g_ctrl_out = 0.0f;
                    break;
            }

            // Escribir a actuation
            g_triac_pwr = (uint8_t)g_ctrl_out;
            g_act_en = true;
        } else {
            g_triac_pwr = 0;
        }

        // Timing
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}