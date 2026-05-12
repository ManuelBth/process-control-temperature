// ============================================================================
// ControlTask - NOT USED FOR NOW (ZcTriacTask is active)
// ============================================================================

// #include <Arduino.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>

// #include "control/drivers/max6675.h"
// #include "control/drivers/triac.h"
// #include "control/drivers/zero_cross.h"
// #include "control/algorithms/pid.h"
// #include "control/task/control_task.h"
// #include "common/data.h"

// extern ControlData g_data;

// static uint32_t s_last_control_time = 0;
// static uint32_t s_last_isr_count = 0;

// void control_task_init() {
//     max6675_init();
//     triac_init();
//     zc_init();
//     pid_init(5.0f, 0.5f, 0.2f);
//     Serial.println("ControlTask: All drivers initialized");
// }

// void ControlTask(void* parameter) {
//     (void)parameter;

//     control_task_init();

//     Serial.println("ControlTask started on Core 0");
//     Serial.flush();

//     s_last_control_time = millis();

//     while (true) {
//         // Debug: verificar si la ISR se ejecuta (aunque esté siendo filtrada)
//         uint32_t current_isr_count = zc_get_isr_count();
//         if (current_isr_count != s_last_isr_count) {
//             Serial.printf("[ZC-DEBUG] ISR triggered! count=%u (diff=%d)\n",
//                          current_isr_count, current_isr_count - s_last_isr_count);
//             s_last_isr_count = current_isr_count;
//         }

//         // Verificar si hubo cruce por cero
//         bool zc_event = zc_detected();
//         if (zc_event) {
//             uint32_t zc_count = zc_get_isr_count();
//             Serial.printf("[ZC] count=%u period=%u us\n",
//                          zc_count, zc_get_period_us());

//             // Disparar TRIAC con el delay actual del PID
//             if (g_data.running) {
//                 triac_fire(g_data.triac_delay_us, g_data.power_percent);
//                 Serial.printf("[TRIAC] fired delay=%u us\n", g_data.triac_delay_us);
//             }
//         }

//         // Control cada 200ms
//         uint32_t now = millis();
//         if (now - s_last_control_time >= 200) {
//             s_last_control_time = now;

//             // Leer temperatura
//             float temp = max6675_read();
//             g_data.sensor_valid = (temp > -50.0f);
//             if (!g_data.sensor_valid) {
//                 temp = 25.0f;
//                 Serial.println("[TEMP] Sensor invalid, using 25.0C");
//             }
//             g_data.temperature = temp;

//             // Calcular PID si está activo
//             if (g_data.pid_active && g_data.running) {
//                 float power = pid_calculate(temp, g_data.setpoint, 0.2f);
//                 g_data.power_percent = power;
//                 // Mapear power (0-100) a delay (8333-100 µs para semiciclo de 60Hz)
//                 // power=0 → delay=8333µs (casi no dispara)
//                 // power=100 → delay=100µs (dispara enseguida)
//                 g_data.triac_delay_us = (uint16_t)map((int)power, 0, 100, 8333, 100);

//                 Serial.printf("[PID] temp=%.1f setpoint=%.1f power=%.1f%% delay=%u us\n",
//                              temp, g_data.setpoint, power, g_data.triac_delay_us);
//             } else {
//                 g_data.power_percent = 0.0f;
//                 g_data.triac_delay_us = 8333;  // No dispara
//                 Serial.println("[PID] OFF");
//             }
//         }

//         vTaskDelay(10 / portTICK_PERIOD_MS);
//     }
// }