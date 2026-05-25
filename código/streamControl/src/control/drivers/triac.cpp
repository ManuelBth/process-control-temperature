// ============================================================================
// triac.cpp — Control de TRIAC MOC3021 + BTA08 (SIN LATCH)
//
// El BTA08 NO tiene latch, por tanto necesita señal continua en el gate
// para mantener la conducción durante todo el tiempo deseado.
//
// Estrategia por semiciclo (120 interrupciones/seg @ 60Hz):
//
//   ZC detectado
//       │
//       ├─→ PIN = LOW  (apagar gate inmediatamente)
//       │
//       ├─→ Calcular delay = semiciclo_us × (1 - power/100)
//       │
//       └─→ Armar esp_timer con ese delay
//                   │
//                   └─→ Timer expira → PIN = HIGH (gate ON, TRIAC conduce)
//                                       │
//                                   Permanece HIGH hasta el próximo ZC
//                                   donde la ISR lo apaga y repite el ciclo
//
// Mapeo power → delay:
//   power = 100% → delay =    0 µs  → casi todo el semiciclo en HIGH
//   power =  50% → delay = semi/2   → mitad del semiciclo en HIGH
//   power =   0% → no se arma timer → pin permanece en LOW
// ============================================================================

#include <Arduino.h>
#include <esp_timer.h>
#include <driver/gpio.h>
#include "control/drivers/triac.h"
#include "common/pins.h"

static esp_timer_handle_t s_timer_on = NULL;

// ─── ISR del timer — pone el gate en HIGH ────────────────────────────────────
// Se ejecuta después del delay calculado. A partir de aquí el BTA08 conduce
// hasta que la ISR del próximo ZC baje el pin.
static void IRAM_ATTR triac_on_cb(void* arg) {
    GPIO.out_w1ts = (1UL << PIN_TRIAC);
}

// ─── Inicialización ──────────────────────────────────────────────────────────
void triac_init() {
    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << PIN_TRIAC);
    cfg.mode         = GPIO_MODE_OUTPUT;
    cfg.pull_up_en   = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type    = GPIO_INTR_DISABLE;
    gpio_config(&cfg);

    // Estado inicial: gate LOW (TRIAC bloqueado)
    GPIO.out_w1tc = (1UL << PIN_TRIAC);

    // Timer de un solo disparo para subir el gate tras el delay
    esp_timer_create_args_t args = {};
    args.callback = &triac_on_cb;
    args.name     = "triac_on";
    esp_timer_create(&args, &s_timer_on);

    Serial.printf("TRIAC init on pin %d (MOC3021 + BTA08, sin latch)\n", PIN_TRIAC);
}

// ─── Llamar desde la ISR del ZC ──────────────────────────────────────────────
// Precondición: la ISR ya bajó el pin a LOW antes de llamar esta función.
//
// Parámetros:
//   semiciclo_us  — período del semiciclo medido por el ZC (µs)
//   power_percent — potencia deseada 0–100
void IRAM_ATTR triac_arm(uint32_t semiciclo_us, uint8_t power_percent) {
    // Cancelar timer anterior por si aún estaba pendiente
    esp_timer_stop(s_timer_on);

    if (power_percent == 0) {
        // Pin ya está en LOW, no armar timer → TRIAC bloqueado todo el semiciclo
        return;
    }

    if (power_percent >= 100) {
        // Potencia máxima: subir el gate prácticamente de inmediato
        // Se usa 50µs de delay mínimo para dar tiempo al circuito ZC
        // a estabilizarse antes de conducir
        esp_timer_start_once(s_timer_on, 50UL);
        return;
    }

    // Calcular delay:
    //   A mayor power → menor delay → más tiempo del semiciclo en HIGH
    //   A menor power → mayor delay → menos tiempo del semiciclo en HIGH
    //
    //   delay_us = semiciclo_us × (1 - power / 100)
    //
    // Ejemplos @ 60Hz (semiciclo = 8333 µs):
    //   power = 80% → delay =  8333 × 0.20 = 1666 µs
    //   power = 50% → delay =  8333 × 0.50 = 4166 µs
    //   power = 20% → delay =  8333 × 0.80 = 6666 µs
    uint32_t delay_us = semiciclo_us * (100U - (uint32_t)power_percent) / 100U;

    esp_timer_start_once(s_timer_on, delay_us);
}