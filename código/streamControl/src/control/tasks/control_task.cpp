// ============================================================================
// CONTROL TASK - Tarea del controlador
// ============================================================================

#include "control_task.h"
#include "control_data.h"
#include "algorithms/pid.h"
#include "sensor_placeholder.h"
#include "../actuation/data/actuation_data.h"

// ============================================================================
// TASK
// ============================================================================

void vControlTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(PID_SAMPLE_TIME_MS);

    // Inicializar sensor
    sensor_init();

    for (;;) {
        // Leer temperatura
        float temp = sensor_read();

        // Guardar temperatura en estado
        g_ctrl_temp = temp;

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
                    // Por implementar
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