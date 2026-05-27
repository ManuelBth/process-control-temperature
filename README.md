# Process Control Temperature

[![Platform](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/)
[![Framework](https://img.shields.io/badge/Framework-Arduino%20%2B%20FreeRTOS-orange.svg)](https://docs.espressif.com/projects/arduino-esp32/)
[![Protocol](https://img.shields.io/badge/BLE-JSON%20GATT-green.svg)](https://www.bluetooth.com/)
[![Build](https://img.shields.io/badge/Build-PlatformIO-brightgreen.svg)](https://platformio.org/)
[![Status](https://img.shields.io/badge/Status-Active%20Development-success.svg)]()
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> **Sistema embebido de control de temperatura para planta térmica académica.** Implementación completa de lazos de control PID/RST sobre ESP32 con FreeRTOS, comunicación inalámbrica BLE, identificación de sistemas y diseño de PCB propio en KiCad.

---

## Tabla de Contenidos

- [Resumen Ejecutivo](#resumen-ejecutivo)
- [Vista General del Sistema](#vista-general-del-sistema)
- [Especificaciones Técnicas](#especificaciones-técnicas)
- [Arquitectura del Firmware](#arquitectura-del-firmware)
- [Hardware](#hardware)
- [Protocolo de Comunicación BLE](#protocolo-de-comunicación-ble)
- [Modelo de Datos Centralizado](#modelo-de-datos-centralizado)
- [Aplicación Móvil (Especificación)](#aplicación-móvil-especificación)
- [Diseño de PCBs](#diseño-de-pcbs)
- [Estructura del Repositorio](#estructura-del-repositorio)
- [Configuración del Entorno de Desarrollo](#configuración-del-entorno-de-desarrollo)
- [Compilación y Carga](#compilación-y-carga)
- [Seguridad y Mecanismos de Failsafe](#seguridad-y-mecanismos-de-failsafe)
- [Rendimiento y Timing](#rendimiento-y-timing)
- [Roadmap](#roadmap)
- [Stack Tecnológico](#stack-tecnológico)
- [Notas Técnicas](#notas-técnicas)
- [Galería](#galería)
- [Referencias](#referencias)

---

## Resumen Ejecutivo

Este proyecto implementa un **sistema de control de procesos térmicos** diseñado para una planta piloto académica. El objetivo es mantener la temperatura de un sistema térmico en un rango operativo de **18°C a 150°C** mediante diferentes estrategias de control clásicas, con capacidad de identificación paramétrica del sistema y supervisión inalámbrica vía Bluetooth Low Energy.

El sistema está construido sobre una arquitectura **bare-metal + RTOS**, donde el firmware corre sobre un **ESP32** utilizando **FreeRTOS** para garantizar determinismo temporal en tareas críticas de control de potencia AC. La interfaz de usuario es una **aplicación Android** (en desarrollo) que actúa como HMI (Human-Machine Interface), permitiendo monitoreo en tiempo real, configuración de parámetros de control, y exportación de datos de sesión.

**Palabras clave:** Control PID, Control RST, Identificación de Sistemas, ESP32, FreeRTOS, BLE GATT, TRIAC, Modulación de Fase, KiCad.

---

## Vista General del Sistema

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                           APLICACIÓN ANDROID (HMI)                            │
│                     Jetpack Compose · Vico Charts · BLE GATT                   │
│                          Min SDK 29 (Android 10+)                              │
└─────────────────────────────────────┬───────────────────────────────────────┘
                                      │ BLE 4.2/5.0
                                      │ JSON over GATT Notify/Write
                                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                         ARDUINO NANO ESP32 (Controlador)                      │
│  ┌─────────────────────────────────────────────────────────────────────────┐ │
│  │                        FREERTOS KERNEL                                   │ │
│  │  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────────────┐   │ │
│  │  │  ControlTask │  │ ZcTriacTask  │  │        LoggerTask            │   │ │
│  │  │  Priority 5  │  │  Priority 5  │  │        Priority 3            │   │ │
│  │  │  Core 0      │  │   Core 1     │  │         Core 0               │   │ │
│  │  │  • PID/RST   │  │  • ISR ZC    │  │  • Serial monitor            │   │ │
│  │  │  • MAX6675   │  │  • TRIAC     │  │  • BLE event detection       │   │ │
│  │  │  • 200 ms    │  │  • Watchdog  │  │  • 1 s telemetry             │   │ │
│  │  └──────────────┘  └──────────────┘  └──────────────────────────────┘   │ │
│  │  ┌──────────────────────────────────────────────────────────────────┐    │ │
│  │  │              NIMBLE BLE SERVER (GATT)                             │    │ │
│  │  │  Service: FFF0  |  RX: FFF1 (Write)  |  TX: FFF2 (Notify)        │    │ │
│  │  └──────────────────────────────────────────────────────────────────┘    │ │
│  └─────────────────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────┬───────────────────────────────────────┘
                                      │ GPIO / SPI / PWM
                                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                              PLANTA TÉRMICA                                   │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐ │
│  │ Termocupla K│  │   TRIAC     │  │  Ventilador │  │   Ventilador 2      │ │
│  │  MAX6675    │  │ MOC3021+    │  │    PWM      │  │     PWM             │ │
│  │  SPI        │  │   BTA08     │  │   25 kHz    │  │   25 kHz            │ │
│  │  -40~150°C  │  │ Phase-firing│  │  Control    │  │  Perturbación       │ │
│  └─────────────┘  └─────────────┘  └─────────────┘  └─────────────────────┘ │
│  ┌─────────────────────────────────────────────────────────────────────────┐ │
│  │              DETECTOR DE CRUCE POR CERO (GPIO 4, ISR RISING)            │ │
│  └─────────────────────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Especificaciones Técnicas

| Parámetro | Especificación |
|-----------|----------------|
| **Rango de control** | 18°C — 150°C (hard limit) |
| **Sensor de temperatura** | Termocupla tipo K + MAX6675 (SPI) |
| **Resolución térmica** | 0.25°C (MAX6675, 12-bit) |
| **Precisión de lectura** | ±2.2°C (0°C — 700°C), ±5°C (700°C — 1000°C) |
| **Actuador térmico** | Resistencia calefactora controlada por TRIAC (modulación de fase) |
| **Control de ventiladores** | PWM 25 kHz, dos canales independientes |
| **Microcontrolador** | Arduino Nano ESP32 (ESP32-WROOM-32) |
| **Frecuencia de muestreo (control)** | 5 Hz (200 ms) |
| **Frecuencia de telemetría BLE** | 1 Hz (paquetes de 10 muestras a 10 Hz) |
| **Stack BLE** | NimBLE-Arduino v1.4.1 |
| **Comunicación inalámbrica** | Bluetooth Low Energy (BLE 4.2/5.0) |
| **Alimentación lógica** | 3.3V (ESP32) / 5V (periféricos) |
| **Frecuencia de red AC** | 50 Hz (Perú/Argentina — período 20 ms, semiciclo 10 ms) |
| **Control de potencia** | 0% — 100% mapeado a delay de disparo 0—10 ms |
| **Framework** | Arduino-ESP32 + FreeRTOS |
| **Flash** | 4 MB |
| **RAM disponible** | ~320 KB |
| **Entorno de desarrollo** | PlatformIO Core / VS Code |

---

## Arquitectura del Firmware

El firmware se organiza en dos proyectos PlatformIO independientes:

### Proyectos

| Proyecto | Ruta | Descripción | Comunicación |
|----------|------|-------------|--------------|
| `streamControl` | `código/streamControl/` | Versión base. FreeRTOS con control PID, TRIAC, y logging por serial. | Serial UART 115200 |
| `streamControlLibrary` | `código/streamControlLibrary/` | Versión avanzada. Agrega BLE NimBLE, parser JSON, máquina de estados, y telemetría estructurada. | BLE GATT + Serial |

### Esquema de Tareas FreeRTOS (`streamControlLibrary`)

| Tarea | Prioridad | Core Pinneado | Período | Stack | Responsabilidad |
|-------|-----------|--------------|---------|-------|-----------------|
| `ControlTask` | 5 | CPU 0 | 200 ms | Calculado | Lectura MAX6675, ejecución PID/RST, actualización de `g_system_data` |
| `ZcTriacTask` | 5 | CPU 1 | Event-driven (ISR) | Calculado | Supervisión de ISR de cruce por cero, actualización atómica de `power_percent`, watchdog de pérdida de señal ZC (>200 ms → failsafe) |
| `LoggerTask` | 3 | CPU 0 | 1000 ms | Calculado | Log por Serial (`115200 baud`), detección de eventos BLE (`start`/`stop`/`config`), estado del sistema |

> **Nota de diseño:** Las tareas de mayor prioridad (`ControlTask` y `ZcTriacTask`) están pinneadas a cores diferentes para evitar contención de CPU entre la lógica de control y las ISR de timing crítico del TRIAC.

### Drivers de Hardware

#### MAX6675 — Sensor de Temperatura
- **Interfaz:** SPI bit-banging (SCK: GPIO 5, CS: GPIO 19, MISO: GPIO 23)
- **Frecuencia de lectura:** Cada 200 ms (ControlTask)
- **Validación:** Si el bit D2 o D0 del frame SPI está activo, se detecta error de sensor. Retorna `-100.0°C` como valor de falla.
- **Filtro:** Promedio móvil simple (configurable en `PRD.md`)

#### TRIAC — Control de Potencia AC
- **Driver:** MOC3021 (optotriac) + BTA08 (triac de potencia)
- **Tipo:** Sin latch (phase-firing)
- **Disparo:** `esp_timer` de un solo disparo (`one-shot`) programado desde la ISR de cruce por cero
- **Mapeo:** `power_percent` (0—100) → `delay_us` inversamente proporcional dentro del semiciclo
- **Seguridad:** Gate se apaga automáticamente en el siguiente cruce por cero (ISR). Watchdog de 200 ms sin ZC → TRIAC apagado.
- **Límite de potencia:** 50% máximo en la implementación actual (línea de seguridad en `pid.cpp`)

#### Zero Cross — Detección de Cruce por Cero
- **Pin:** GPIO 4
- **Modo de interrupción:** `RISING` (cruce ascendente)
- **ISR (`zc_isr`):**
  1. Apaga inmediatamente el gate del TRIAC (corte seguro)
  2. Mide el período del semiciclo (validación: 7000—11000 µs)
  3. Programa el disparo del TRIAC vía `triac_arm(semiciclo_us, power_percent)`
- **Acceso concurrente:** Variables compartidas (`power_percent`, `period_us`) protegidas con acceso atómico (`__atomic_*`)

#### Fan (PWM) — Ventiladores
- **Pines:** GPIO 16 (Fan 1), GPIO 17 (Fan 2)
- **Frecuencia PWM:** 25 kHz (compatible con ventiladores de PC de 4 pines)
- **Resolución:** 8-bit (0—255)
- **Modos:**
  - `FAN_SPEED_NORMAL` = 70% (base)
  - `FAN_SPEED_PERTURBATION` = 100% (modo perturbación)
- **Compatibilidad:** API LEDC legacy y moderna (ESP32 core 2.x y 3.0+)

### Algoritmos de Control

#### PID (Proporcional-Integral-Derivativo)
Implementación en `código/streamControlLibrary/src/algorithms/pid.cpp`:

```cpp
u(t) = Kp·e(t) + Ki·∫e(t)dt + Kd·de(t)/dt
```

- **Parámetros por defecto:** Kp = 5.0, Ki = 0.5, Kd = 0.2
- **Anti-windup:** Clamping del término integral a ±1000
- **Reset condicional:** Si `temperatura > setpoint`, el integral se resetea a 0 y la salida es 0 (protección contra sobrecalentamiento)
- **Límite de salida:** 0% — 50% (hard limit en firmware)
- **Actualización en caliente:** Parámetros configurables vía mensaje BLE `"op": "set"` (configuración PID)

#### RST (Planificado — placeholder en estructuras)
- Estructuras `r_coeffs[3]`, `s_coeffs[3]`, `t_coeffs[3]` reservadas en `central_data_t`
- Diseño basado en modelo identificado del sistema
- Implementación pendiente en roadmap

#### Identificación del Sistema
- **Respuesta escalón:** Aplicar cambio escalón en potencia, registrar curva de temperatura, extraer K, τ, θ
- **PRBS:** Señal binaria pseudo-aleatoria para excitar múltiples frecuencias. Algoritmo de mínimos cuadrados.
- **Modelo objetivo:** Primer orden con retardo — `G(s) = K·e^(-θs) / (τs + 1)`
- **Estados:** `IDENT_IDLE`, `IDENT_RUNNING`, `IDENT_COMPLETE`, `IDENT_ERROR`

---

## Hardware

### Pinout del ESP32

| Pin GPIO | Función | Dirección | Descripción |
|----------|---------|-----------|-------------|
| GPIO 4 | Zero Cross | Input (ISR) | Detector de cruce por cero de red AC 50Hz |
| GPIO 5 | SPI SCK | Output | Clock del MAX6675 |
| GPIO 15 | TRIAC Gate | Output | Disparo del optotriac MOC3021 |
| GPIO 16 | Fan 1 PWM | Output | Ventilador de control (PWM 25kHz) |
| GPIO 17 | Fan 2 PWM | Output | Ventilador de perturbación (PWM 25kHz) |
| GPIO 19 | SPI CS | Output | Chip Select del MAX6675 |
| GPIO 23 | SPI MISO | Input | Datos del MAX6675 |

### Diagrama de Conexiones

```
ESP32 (3.3V)
│
├── GPIO 4  ─────────────►  Detector de Cruce por Cero (optocoplador)
├── GPIO 15 ─────────────►  MOC3021 ──► Gate BTA08 (TRIAC)
├── GPIO 5  ─────────────►  SCK  MAX6675
├── GPIO 19 ─────────────►  CS   MAX6675
├── GPIO 23 ─────────────◄  MISO MAX6675
├── GPIO 16 ─────────────►  Fan 1 (PWM 25kHz)
└── GPIO 17 ─────────────►  Fan 2 (PWM 25kHz)

AC 220V
│
├── Fase  ───────────────►  BTA08 (A1)
├── Neutro ──────────────►  BTA08 (A2) ──► Resistencia Calefactora
└── Detector ZC ─────────►  Optocoplador (con divisor resistivo)
```

### Componentes Principales

| Componente | Modelo | Especificación |
|------------|--------|----------------|
| Microcontrolador | ESP32-WROOM-32 | Dual-core 240 MHz, 4 MB Flash, WiFi + BLE |
| Sensor térmico | MAX6675 + Termocupla K | -40°C a +1024°C, resolución 0.25°C |
| Optotriac | MOC3021 | Sin latch, cero-cross o phase-firing |
| Triac de potencia | BTA08-600 | 8 A, 600 V, aislado |
| Ventiladores | PC 4-pin | PWM 25 kHz, 12 V |

> **Sensor:** El sistema utiliza una **termocupla tipo K** con interfaz MAX6675 (SPI) para lectura de temperatura en el rango de -40°C a +1024°C.

---

## Protocolo de Comunicación BLE

> **Protocolo oficial:** El firmware implementa el protocolo definido en [`app/PRD.md`](app/PRD.md).

### Servicio GATT

| Parámetro | Valor |
|-----------|-------|
| **Nombre del dispositivo** | `StreamControl` |
| **UUID del servicio** | `0000FFF0-0000-1000-8000-00805F9B34FB` |
| **UUID RX (App → ESP32)** | `0000FFF1-0000-1000-8000-00805F9B34FB` (Write) |
| **UUID TX (ESP32 → App)** | `0000FFF2-0000-1000-8000-00805F9B34FB` (Notify) |
| **MTU negociado** | 512 bytes (requerido por la app para evitar fragmentación) |
| **Intervalo de advertising** | 100 ms |
| **Intervalo de conexión** | 7.5 — 10 ms |
| **Timeout de conexión** | 5000 ms |

### Tipos de Mensaje (v2.0 — Oficial)

#### 1. Telemetría — ESP32 → App (Notify)

**Frecuencia:** 1 Hz

| Campo | Clave JSON | Tipo | Rango | Descripción |
|-------|-----------|------|-------|-------------|
| Timestamp | `t` | long | ms | Tiempo en ms desde el inicio del sistema |
| Temperatura | `p100` | float | °C | Temperatura actual de la termocupla tipo K |
| Estado | `st` | int | 0, 1, 2 | `0`=Stop, `1`=Run, `2`=Overheat |
| PWM Resistencia | `pwm_h` | int | 0 — 255 | Feedback del PWM actual de la resistencia calefactora |

**Ejemplo:**

```json
{
  "t":    450012,
  "p100": 38.52,
  "st":   1,
  "pwm_h": 120
}
```

> El parser de la app usa `kotlinx.serialization` con `ignoreUnknownKeys = true` para tolerar campos experimentales del firmware.

#### 2. Control Manual — App → ESP32 (Write)

> **Condición:** Enviado cuando la app está en modo Manual.

| Campo | Clave | Tipo | Rango | Descripción |
|-------|-------|------|-------|-------------|
| Operación | `op` | string | `"manual"` | Modo manual |
| Ventiladores | `fans` | int[2] | 0 — 255 | `[fan1, fan2]` PWM |
| Resistencia | `heat` | int | 0 — 255 | PWM de la resistencia calefactora |
| AC | `ac` | bool | true/false | Corte físico de corriente AC |

**Ejemplo:**

```json
{
  "op":   "manual",
  "fans": [128, 60],
  "heat": 200,
  "ac":   true
}
```

#### 3. Inicio de Control PID — App → ESP32 (Write)

> **Condición:** Enviado al presionar "Iniciar Control" en modo PID.

| Campo | Clave | Tipo | Rango | Descripción |
|-------|-------|------|-------|-------------|
| Operación | `op` | string | `"set"` | Iniciar lazo PID |
| AC | `ac` | bool | true/false | Habilitar corriente AC |
| PID | `pid` | object | — | Parámetros del controlador |
| PID · Setpoint | `pid.sp` | float | °C | Temperatura objetivo |
| PID · Kp | `pid.kp` | float | ≥ 0 | Ganancia proporcional |
| PID · Ki | `pid.ki` | float | ≥ 0 | Ganancia integral |
| PID · Kd | `pid.kd` | float | ≥ 0 | Ganancia derivativa |
| PID · Ts | `pid.ts` | int | 50—5000 ms | Tiempo de muestreo |
| PID · Ttotal | `pid.ttotal` | int | > 0 s | Duración planificada del ensayo |

**Ejemplo:**

```json
{
  "op": "set",
  "ac": true,
  "pid": {
    "sp":     75.0,
    "kp":     1.5,
    "ki":     0.05,
    "kd":     0.1,
    "ts":     100,
    "ttotal": 300
  }
}
```

#### 4. Perturbación de Ventiladores — App → ESP32 (Write)

> **Condición:** Enviado durante modo PID para introducir perturbaciones externas.

| Campo | Clave | Tipo | Rango | Descripción |
|-------|-------|------|-------|-------------|
| Operación | `op` | string | `"perturb"` | Perturbación |
| Ventiladores | `fans` | int[2] | 0 — 255 | `[fan1, fan2]` PWM |

**Ejemplo:**

```json
{
  "op":   "perturb",
  "fans": [200, 200]
}
```

#### 5. Detener Operación — App → ESP32 (Write)

**Ejemplo:**

```json
{"op": "stop"}
```

---

## Modelo de Datos Centralizado

El firmware implementa una **única fuente de verdad** mediante la estructura `central_data_t`, definida en `código/streamControlLibrary/src/common/data/central_data.h`.

### Estructura principal (`central_data_t`)

```c
typedef struct {
    sensor_data_t         sensor;         // Datos de sensores
    control_data_t        control;        // Datos de control (PID/RST)
    actuation_data_t      actuation;      // Datos de actuadores
    identification_data_t identification; // Datos de identificación
    display_data_t        display;        // Datos del display LCD
    comm_data_t           comm;           // Datos de comunicación
    system_flags_t        flags;          // Flags globales (bitfield)
} central_data_t;
```

### Tipos base

| Tipo | Definición | Rango | Uso |
|------|-----------|-------|-----|
| `temperature_raw_t` | `int16_t` | -4000 a 15000 | Temperatura × 100 (resolución 0.01°C) |
| `percent_t` | `uint8_t` | 0 — 100 | Porcentajes (potencia, velocidad) |
| `timestamp_t` | `uint32_t` | — | Timestamps Unix (overflow-safe hasta 2038) |
| `millis_t` | `uint32_t` | — | Timestamps desde boot (overflow-safe) |

### Modos de control (`control_mode_t`)

| Valor | Modo | Descripción |
|-------|------|-------------|
| `0` | `CONTROL_MODE_OFF` | Sistema apagado, actuadores en estado seguro |
| `1` | `CONTROL_MODE_MANUAL` | Control directo de actuadores desde la app |
| `2` | `CONTROL_MODE_PID` | Lazo cerrado PID automático |
| `3` | `CONTROL_MODE_RST` | Controlador RST digital (planificado) |

### Macros de acceso rápido

El sistema expone macros para acceso directo a variables críticas sin necesidad de navegar la estructura anidada:

```c
#define g_temp          g_system_data.sensor.temperature
#define g_mode          g_system_data.control.mode
#define g_setpoint      g_system_data.control.setpoint
#define g_triac_power   g_system_data.actuation.triac_power
#define g_fan1_speed    g_system_data.actuation.fan1_speed
```

> **Diseño:** Toda la comunicación inter-tareas se realiza mediante acceso atómico a `g_system_data`, garantizando consistencia en tiempo real sin overhead de colas para datos de control críticos.

---

## Aplicación Móvil (Especificación)

> **Estado:** Especificación completa. Código fuente Kotlin no incluido en este repositorio.

La aplicación Android actúa como HMI y está especificada en `app/PRD.md`. Arquitectura propuesta:

### Stack de la App

| Capa | Tecnología |
|------|------------|
| Lenguaje | Kotlin 2.0.0 |
| UI | Jetpack Compose (Single Activity) |
| Min/Target SDK | API 29 / API 35 |
| Gráficas | Vico Charts (`com.patrykandpatrick.vico`) |
| BLE | Android BluetoothLeScanner + BluetoothGatt |
| Serialización | Kotlinx Serialization JSON |
| Inyección de dependencias | Koin 3.5.6 |
| Concurrencia | Kotlin Coroutines + Flow |

### Arquitectura

```
┌──────────────────────────────────────────────────────────────┐
│                        UI Layer (Compose)                     │
│  ConnectionHeader · LiveChartSection · ControlPanel · Footer  │
└──────────────────────────┬───────────────────────────────────┘
                           │ StateFlow<T>
                           ▼
┌──────────────────────────────────────────────────────────────┐
│                     ViewModel Layer                           │
│  MonitoringViewModel · ControlViewModel · SessionViewModel    │
└──────────────────────────┬───────────────────────────────────┘
                           │ UseCase
                           ▼
┌──────────────────────────────────────────────────────────────┐
│                    Domain Layer                               │
│  TelemetryData · PidParameters · ControlCommand · AppMode     │
└──────────────────────────┬───────────────────────────────────┘
                           │ Repository Interface
                           ▼
┌──────────────────────────────────────────────────────────────┐
│                     Data Layer                                │
│  BluetoothRepositoryImpl · StorageRepositoryImpl              │
│  TelemetryParser (JSON → Domain) · LogFileWriter (CSV)        │
└──────────────────────────┬───────────────────────────────────┘
                           │ BLE GATT
                           ▼
┌──────────────────────────────────────────────────────────────┐
│                     ESP32 (Firmware)                          │
└──────────────────────────────────────────────────────────────┘
```

### Modos de Operación de la App

| Modo | Control de actuadores | Configuración PID | Perturbación (fans) |
|------|----------------------|-------------------|---------------------|
| **Manual** | Sliders PWM directos (heat, f1, f2) + Switch AC | Deshabilitado | Habilitado |
| **Control PID** | Deshabilitado (ESP32 controla internamente) | Setpoint, Kp, Ki, Kd, Ts, Ttotal | Habilitado (perturbación externa) |

### Exportación de Sesiones

- **Formato:** CSV con encabezado de metadatos
- **Nomenclatura:** `LOG_YYYYMMDD_HHMMSS.txt`
- **Ubicación:** `Android/data/com.app.thermalcontrol/files/sessions/`
- **Contenido:** Timestamp, temperatura, PWMs, estado AC, estado del sistema
- **Share:** Compartible vía `ShareCompat` (Drive, WhatsApp, email)

### Máquina de Estados BLE

```
DISCONNECTED ──scan()──► SCANNING ──device found──► CONNECTING
    ▲                                                    │
    │                GATT connected                      │ RSSI ≥ -80 dBm
    │                      ▼                             │ (auto-reconnect)
    │               CONNECTED_READY ◄────────────────────┘
    │                      │
    │         start control / manual
    │                      ▼
    └────────────── RUNNING (data flow active)
                           │
                    stop / error
                           ▼
                    CONNECTED_READY
```

---

## Diseño de PCBs

Los diseños electrónicos se realizan en **KiCad 7.x/8.x** y se encuentran en `circuitos/`.

| Proyecto KiCad | Ubicación | Descripción | Estado |
|---------------|-----------|-------------|--------|
| Adaptador | `circuitos/adaptador/` | PCB adaptadora con footprints personalizados | Gerbers listos |
| Circuito de Potencia | `circuitos/circuito-potencia/` | TRIAC, alimentación AC, protecciones | Diseño en curso |
| Control TRIAC+ZC | `circuitos/PCB/CONTROLTRIACYCRUCE0/` | PCB principal del controlador TRIAC + cruce por cero | Diseño en curso |
| Control ESP32 | `circuitos/PCB/controlesp32/` | PCB del controlador basado en ESP32 | Diseño en curso |

### Archivos de Fabricación

Los gerbers del adaptador están generados y listos para envío a fabricación:

```
circuitos/adaptador-gerber/
├── adaptador-B_Cu.gbr      (Capa inferior - cobre)
├── adaptador-Edge_Cuts.gbr (Contorno de la PCB)
└── adaptador-job.gbrjob    (Job file KiCad)
```

---

## Estructura del Repositorio

```
process-control-temperature/
│
├── README.md                          # Este documento
├── PRD.md                             # Product Requirements Document (sistema completo)
├── .gitignore                         # Exclusiones Git (Node/Angular legacy)
│
├── app/                               # Especificación de la App Android
│   ├── PRD.md                         # PRD v2.0 — Arquitectura MVVM + UDF
│   └── mensajes.md                    # Copia de referencia del protocolo BLE
│
├── circuitos/                         # Diseños electrónicos (KiCad)
│   ├── adaptador/                     # PCB adaptadora + backups KiCad
│   ├── adaptador-gerber/              # Gerbers listos para fabricación
│   ├── circuito-potencia/             # Circuito de potencia (TRIAC, PSU)
│   ├── PCB/                           # PCB principal del controlador
│   │   ├── CONTROLTRIACYCRUCE0.*      # Esquemático y PCB del controlador
│   │   └── controlesp32/              # PCB del módulo ESP32
│   └── files/                         # Footprints y modelos 3D
│
├── código/                            # Firmware ESP32 (PlatformIO)
│   ├── PRD_SYSTEM_ESP32.md            # Requerimientos del firmware ESP32
│   │
│   ├── streamControl/                 # Proyecto base (FreeRTOS, sin BLE)
│   │   ├── platformio.ini             # Configuración: espressif32, esp32dev
│   │   ├── src/                       # Código fuente (.cpp)
│   │   │   ├── common/data.cpp
│   │   │   ├── control/algorithms/pid.cpp
│   │   │   ├── control/drivers/
│   │   │   │   ├── max6675.cpp
│   │   │   │   ├── triac.cpp, triac.h
│   │   │   │   └── zero_cross.cpp, zero_cross.h
│   │   │   ├── control/task/
│   │   │   │   ├── control_task.cpp
│   │   │   │   ├── logger_task.cpp
│   │   │   │   └── zc_triac_task.cpp
│   │   │   └── main.cpp
│   │   └── include/                   # Headers (.h)
│   │       ├── common/data.h, pins.h
│   │       ├── control/algorithms/pid.h
│   │       └── control/drivers/
│   │           ├── max6675.h, triac.h, zero_cross.h
│   │           └── control/task/
│   │               ├── control_task.h, logger_task.h, zc_triac_task.h
│   │
│   ├── streamControlLibrary/          # Proyecto avanzado (BLE + JSON + State Machine)
│   │   ├── platformio.ini             # + NimBLE-Arduino@^1.4.1
│   │   ├── data/                      # (reservado)
│   │   ├── src/
│   │   │   ├── algorithms/pid.cpp
│   │   │   ├── ble/
│   │   │   │   ├── ble_config.h       # UUIDs GATT, constantes BLE
│   │   │   │   ├── ble_handler.cpp/h  # Servidor NimBLE
│   │   │   │   ├── fake_data.cpp/h    # Generador de muestras de telemetría
│   │   │   │   ├── message_parser.cpp/h  # Parser JSON (cJSON)
│   │   │   │   ├── message_types.h    # Tipos de mensaje
│   │   │   │   └── state_machine.cpp/h   # Máquina de estados (IDLE/RUNNING)
│   │   │   ├── common/
│   │   │   │   ├── data/
│   │   │   │   │   ├── central_data.h  # Única fuente de verdad
│   │   │   │   │   └── data.cpp
│   │   │   │   └── pins/
│   │   │   │       ├── pins.cpp
│   │   │   │       └── pins.h         # Definición de pines GPIO
│   │   │   ├── drivers/
│   │   │   │   ├── fan.cpp, fan.h
│   │   │   │   ├── max6675.cpp, max6675.h
│   │   │   │   ├── triac.cpp, triac.h
│   │   │   │   └── zero_cross.cpp, zero_cross.h
│   │   │   ├── main.cpp
│   │   │   └── task/
│   │   │       ├── control_task.cpp
│   │   │       ├── logger_task.cpp
│   │   │       └── zc_triac_task.cpp
│   │   ├── include/                   # Headers públicos
│   │   └── lib/
│   │       └── cJSON/                 # Librería JSON embebida (cJSON.c, cJSON.h)
│   │
│   └── Backup/                        # Versiones anteriores del firmware
│       ├── Funcionando/               # Snapshot funcional con estructura completa
│       ├── Latch/                     # Versión con TRIAC con latch
│       └── Sin Latch/                 # Versión con TRIAC sin latch
│
└── simulaciones/                      # (Reservado para modelos MATLAB/Simulink/Python)
```

---

## Configuración del Entorno de Desarrollo

### Requisitos

| Herramienta | Versión mínima | Propósito |
|-------------|---------------|-----------|
| PlatformIO Core | 6.0 | Compilación, carga y monitor serial |
| Python | 3.9 | Dependencia de PlatformIO |
| Git | 2.30 | Control de versiones |
| VS Code (opcional) | 1.80 | IDE con extensión PlatformIO |
| KiCad (opcional) | 7.0 | Edición de esquemáticos y PCBs |

### Instalación de PlatformIO

```bash
# Instalar PlatformIO Core
pip install platformio

# Verificar instalación
pio --version
```

### Estructura de `platformio.ini`

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600
lib_deps =
    h2zero/NimBLE-Arduino@^1.4.1
```

| Parámetro | Valor | Descripción |
|-----------|-------|-------------|
| `platform` | `espressif32` | Plataforma ESP32 de Espressif |
| `board` | `esp32dev` | DevKit genérico ESP32-WROOM-32 |
| `framework` | `arduino` | Framework Arduino-ESP32 (incluye FreeRTOS) |
| `monitor_speed` | `115200` | Velocidad del monitor serial (baudios) |
| `upload_speed` | `921600` | Velocidad de carga (programación) |
| `lib_deps` | `NimBLE-Arduino@^1.4.1` | Stack BLE ligero (solo `streamControlLibrary`) |

---

## Compilación y Carga

### Firmware Avanzado (con BLE)

```bash
# Navegar al proyecto
cd código/streamControlLibrary

# Compilar el firmware
pio run

# Cargar al ESP32 conectado por USB
pio run --target upload

# Abrir monitor serial (Ctrl+C para salir)
pio device monitor --baud 115200
```

### Firmware Base (sin BLE)

```bash
cd código/streamControl

# Compilar y cargar
pio run --target upload

# Monitor serial
pio device monitor --baud 115200
```

### Verificación post-carga

Tras cargar el firmware avanzado, el ESP32 debe:

1. Iniciar advertising BLE con nombre `StreamControl`
2. Publicar logs por serial cada 1 segundo (`LoggerTask`)
3. Responder a conexiones GATT en el servicio `FFF0`

---

## Seguridad y Mecanismos de Failsafe

El sistema implementa múltiples capas de seguridad para prevenir sobretemperatura y condiciones de falla.

### 1. Watchdog de Comunicación BLE

| Condición | Acción |
|-----------|--------|
| Sin mensajes BLE en 5 segundos | Corta AC (`ac:0`), fuerza `heat:0`, transiciona a estado de alarma (`st:2`) |
| Heartbeat cesa | Activa failsafe completo |

### 2. Heartbeat de Seguridad

Durante operación activa (modo Manual o PID), la App debe enviar un mensaje de control válido al menos cada **1 segundo** para alimentar el watchdog de comunicación.

```json
{"op": "manual", "fans": [128,60], "heat": 200, "ac": true}   // Heartbeat en modo manual
```

### 3. Protección por Sobretemperatura (>150°C)

| Secuencia | Acción |
|-----------|--------|
| 1 | Cortar PWM de la resistencia (0%) |
| 2 | Ambos ventiladores a 100% |
| 3 | Cortar relay principal (off) |
| 4 | Cambiar a modo `OFF` |
| 5 | Enviar mensaje de error por serial/BLE |
| 6 | Mostrar "ERR" en display LCD |

### 4. Validación de Sensor MAX6675

| Condición | Acción |
|-----------|--------|
| Bit D2 o D0 activo en frame SPI | Muestra descartada, se envía último valor válido en telemetría |
| 3 errores consecutivos | Transiciona a `st:2` (Overheat), activa failsafe |

### 5. Watchdog de Cruce por Cero

| Condición | Acción |
|-----------|--------|
| Sin detección de ZC en 200 ms | Apaga TRIAC inmediatamente |
| Período de semiciclo fuera de rango (7000—11000 µs) | Descarta muestra, mantiene estado anterior |

### 6. Límites del Controlador PID

| Límite | Valor | Propósito |
|--------|-------|-----------|
| Salida máxima | 50% | Prevenir sobrecalentamiento por acción de control excesiva |
| Integral clamp | ±1000 | Anti-windup |
| Reset de integral | Temperatura > Setpoint | Protección pasiva ante overshoot |

---

## Rendimiento y Timing

| Parámetro | Valor | Notas |
|-----------|-------|-------|
| **Loop de control** | 200 ms (5 Hz) | ControlTask — lectura + PID |
| **Muestreo interno** | 100 ms (10 Hz) | Submuestreo para telemetría batch |
| **Telemetría BLE** | 1 Hz | Paquetes de 10 muestras |
| **Logging serial** | 1 Hz | LoggerTask |
| **ISR de cruce por cero** | < 50 µs | Tiempo de ejecución de la ISR |
| **Delay de disparo TRIAC** | 0 — 10 ms | Mapeado a 0% — 100% de potencia |
| **Precisión de timing** | < 1 ms | Validación del semiciclo AC |
| **Latencia de parseo JSON** | < 10 ms (objetivo app) | Requisito no funcional de la app |
| **MTU BLE** | 512 bytes | Negociado por la app para evitar fragmentación de JSON |

---

## Roadmap

### Completado

- [x] Firmware base FreeRTOS con control PID
- [x] Driver TRIAC con detección de cruce por cero (sin latch)
- [x] Driver MAX6675 (SPI bit-banging)
- [x] Comunicación BLE NimBLE con protocolo JSON v2.0.0
- [x] Máquina de estados (IDLE / RUNNING)
- [x] Parser de mensajes JSON (cJSON)
- [x] Estructura de datos centralizada (`central_data_t`)
- [x] Control PWM de ventiladores (25 kHz, dos canales)
- [x] Diseño de PCBs en KiCad (adaptador, potencia, control)
- [x] Generación de Gerbers para fabricación
- [x] Especificación completa de app Android (PRD v2.0)

### En Desarrollo / Pendiente

- [ ] Implementación de controlador RST
- [ ] Identificación del sistema: respuesta escalón automatizada
- [ ] Identificación del sistema: señal PRBS + mínimos cuadrados
- [ ] Sintonización automática PID (Ziegler-Nichols, Cohen-Coon)
- [ ] Aplicación Android (Jetpack Compose + Vico Charts)
- [ ] Exportación de sesiones a CSV desde la app
- [ ] Implementación del display LCD 16x2
- [ ] Protocolo serial detallado (UART 115200)
- [ ] Guardado de configuración en EEPROM/Flash
- [ ] Simulaciones del sistema (MATLAB/Simulink o Python)
- [ ] Integración y pruebas de sistema end-to-end

---

## Stack Tecnológico

### Firmware (Embedded)

| Componente | Tecnología | Versión |
|------------|-----------|---------|
| MCU | ESP32-WROOM-32 | — |
| Framework | Arduino-ESP32 | Latest stable |
| RTOS | FreeRTOS (incluido en Arduino-ESP32) | — |
| BLE Stack | NimBLE-Arduino | ^1.4.1 |
| Serialización JSON | cJSON (embedded) | 1.7.x |
| Build System | PlatformIO | ^6.0 |
| IDE | VS Code + PlatformIO Extension | — |

### Hardware

| Componente | Herramienta |
|------------|-------------|
| Esquemáticos | KiCad 7.x/8.x |
| Simulación | (Pendiente: LTspice / Proteus) |

### Aplicación Móvil (Planificada)

| Componente | Tecnología |
|------------|------------|
| Lenguaje | Kotlin 2.0.0 |
| UI Framework | Jetpack Compose |
| Gráficas | Vico Charts 1.15.0 |
| BLE | Android BluetoothGatt |
| Serialización | Kotlinx Serialization JSON 1.7.0 |
| DI | Koin 3.5.6 |
| Async | Kotlin Coroutines 1.8.1 |

---

## Notas Técnicas

### Estructura de Datos Compartida

> La estructura `central_data_t` es la **única fuente de verdad** del sistema. No se utilizan colas FreeRTOS para datos de control crítico; en su lugar, se emplea acceso atómico (`__atomic_*`) y bitfields para garantizar consistencia entre tareas e ISR sin introducir latencia de colas.

### Compatibilidad del PWM

> El driver de ventiladores (`fan.cpp`) es compatible con ambas APIs LEDC de Arduino-ESP32: la API legacy (core 2.x) y la API moderna (core 3.0+), asegurando portabilidad entre versiones del framework.

---

## Referencias

### Bibliografía de Control

- Ziegler, J.G., Nichols, N.B. (1942). *"Optimum Settings for Automatic Controllers"*
- Cohen, G.H., Coon, G.A. (1953). *"Theoretical Consideration of Retarded Control"*
- Åström, K.J., Hägglund, T. (1995). *"PID Controllers: Theory, Design, and Tuning"*
- Franklin, G.F., et al. (2015). *"Feedback Control of Dynamic Systems"*

### Documentación Técnica del Proyecto

| Documento | Ubicación | Contenido |
|-----------|-----------|-----------|
| PRD Sistema | [`PRD.md`](PRD.md) | Arquitectura, modos de control, algoritmos, requerimientos, seguridad |
| PRD Firmware | [`código/PRD_SYSTEM_ESP32.md`](código/PRD_SYSTEM_ESP32.md) | Justificación de RTOS, tareas propuestas, comunicación serial |
| PRD App Android (incluye protocolo BLE oficial) | [`app/PRD.md`](app/PRD.md) | Arquitectura MVVM + UDF, componentes Compose, máquina de estados BLE, protocolo JSON v2.0 |

### Recursos Externos

- [Arduino-ESP32 Documentation](https://docs.espressif.com/projects/arduino-esp32/)
- [FreeRTOS Kernel Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [NimBLE-Arduino GitHub](https://github.com/h2zero/NimBLE-Arduino)
- [cJSON GitHub](https://github.com/DaveGamble/cJSON)
- [KiCad Documentation](https://docs.kicad.org/)
- [Jetpack Compose](https://developer.android.com/jetpack/compose)
- [Vico Charts](https://patrykandpatrick.com/vico/)

---

## Galería

> **Nota:** Las fotos del hardware ensamblado se agregarán en próximas iteraciones.

### Planta Térmica

![Planta térmica ensamblada](docs/images/planta_thermal.jpg)

*Vista general de la planta térmica con resistencia calefactora, ventiladores y sensor de temperatura.*

---

### PCB del Controlador

![PCB del controlador ESP32](docs/images/pcb_controlador.jpg)

*PCB del controlador basado en ESP32 con interfaz MAX6675, driver TRIAC y conectores de ventiladores.*

---

### Aplicación Android

![App Android - Modo Control PID](docs/images/app_pid_mode.png)

*Interfaz de la app en modo Control PID mostrando gráfica en tiempo real (Vico Charts) y panel de configuración.*

---

### Diagrama Esquemático

![Esquemático del circuito de potencia](docs/images/esquematico_potencia.png)

*Esquemático del circuito de potencia: TRIAC, detector de cruce por cero y protecciones.*

---

## Licencia

Este proyecto es de uso académico y educativo. La licencia específica está por definir. Consulte el archivo `LICENSE` (cuando esté disponible) para los términos completos.

---

> **Nota para colaboradores:** Si encontrás inconsistencias entre el firmware y la documentación, o si detectás comportamientos inesperados en el control de potencia del TRIAC, por favor abrí un issue detallando la condición de prueba, el estado del sistema (`g_system_data`), y las tramas JSON intercambiadas.

---

<p align="center">
  <i>Proyecto de Control de Procesos — Ingeniería en Sistemas Embebidos</i>
</p>
