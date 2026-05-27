# PRD v2.0 — Aplicación Móvil de Monitoreo y Control Térmico
**Arquitectura:** Screaming Architecture + MVVM + UDF
**Framework UI:** Jetpack Compose (Single Activity)
**Min SDK:** API 29 (Android 10)
**Target SDK:** API 35
**Versión del documento:** 2.0
**Última actualización:** 2025-07

---

## 1. Objetivo del Producto

Proporcionar una interfaz HMI (Human-Machine Interface) robusta para el monitoreo y control de procesos térmicos sobre una planta con un sensor PT100 y una ESP32 como unidad de control. La app permite al operador alternar entre un **modo manual** (control directo de actuadores) y un **modo control de proceso** (lazo cerrado PID ejecutado en la ESP32), visualizar telemetría en tiempo real, introducir perturbaciones mediante los ventiladores y exportar registros de sesión completos para análisis posterior.

---

## 2. Pilares Arquitectónicos

### 2.1 Screaming Architecture

El proyecto se organiza por intenciones de negocio, no por capas técnicas. Al abrir el proyecto, cualquier desarrollador sabrá inmediatamente que controla una planta térmica.

```
com.app.thermalcontrol
├── core
│   ├── bluetooth          # Low-level GATT, escaneo, reconexión automática
│   ├── storage            # FileSystem API, BufferedWriter, estrategia de flush
│   └── util               # Extensiones Kotlin, parser JSON, formateadores
│
├── domain
│   ├── model              # Data classes: TelemetryData, PidParameters, ControlCommand, AppMode
│   ├── repository         # Interfaces: IThermalRepository, IStorageRepository
│   └── usecase            # SendControlCommandUseCase, ExportLogUseCase, SetOperationModeUseCase
│
├── data
│   ├── repository         # BluetoothRepositoryImpl, StorageRepositoryImpl
│   ├── local              # LogFileWriter (Dispatchers.IO)
│   └── remote             # TelemetryParser: JSON ESP32 → modelos de dominio
│
├── features
│   ├── monitoring         # Gráfica en tiempo real, telemetría PT100
│   │   ├── ui             # MonitoringScreen, MonitoringViewModel
│   │   └── components     # RealTimeChart, TelemetryCard, FreezeButton
│   ├── control            # Panel de actuadores y PID
│   │   ├── ui             # ControlScreen, ControlViewModel
│   │   └── components     # PwmSlider, PidInputField, AcMasterSwitch, ModeToggle
│   └── session            # Inicio/parada de grabación y exportación
│       ├── ui             # SessionViewModel
│       └── components     # RecordingControls, ExportButton, SessionSummary
│
└── ui
    ├── theme              # Color.kt, Type.kt, Shape.kt (Design System)
    ├── navigation         # AppNavGraph.kt, Screen.kt
    └── MainActivity.kt    # Entry point, NavHost, SingleActivity
```

### 2.2 Patrón Component / Presentational (Stateless UI)

- **Screen Containers (Smart):** conocen el `ViewModel`, recolectan el `StateFlow` y poseen el estado de UI.
- **Stateless Components (Dumb):** reciben valores como parámetros y emiten callbacks. No conocen la lógica de negocio.

**Ejemplos:**

| Componente | Tipo | Props clave | Callbacks |
|---|---|---|---|
| `RealTimeChart` | Stateless | `data: List<TelemetryPoint>`, `isFrozen: Boolean` | — |
| `PwmSlider` | Stateless | `value: Int`, `range: IntRange`, `label: String` | `onValueChange: (Int) -> Unit` |
| `PidInputField` | Stateless | `value: String`, `label: String` | `onValueChange: (String) -> Unit` |
| `AcMasterSwitch` | Stateless | `isOn: Boolean` | `onToggle: (Boolean) -> Unit` |
| `ModeToggle` | Stateless | `currentMode: AppMode` | `onModeChange: (AppMode) -> Unit` |

### 2.3 Unidirectional Data Flow (UDF)

```
ESP32 (BLE GATT Notify)
        │
        ▼
BluetoothGattManager  ──►  TelemetryParser  ──►  BluetoothRepositoryImpl
                                                         │
                                              SharedFlow<TelemetryData>
                                                         │
                                              ┌──────────┴──────────┐
                                              ▼                     ▼
                                   MonitoringViewModel      SessionViewModel
                                              │
                                   StateFlow<MonitoringUiState>
                                              │
                                              ▼
                                     MonitoringScreen
                                              │
                                    Stateless Components
```

La UI nunca solicita datos activamente; reacciona a los cambios de estado publicados por el `ViewModel`.

---

## 3. Modos de Operación

Esta es la funcionalidad central del producto. El usuario puede alternar entre dos modos en cualquier momento desde el `ControlPanel`.

### 3.1 Modo Manual

El operador controla directamente cada actuador de la planta.

**Controles disponibles:**
- **Slider PWM — Resistencia de calefacción:** rango `[0, 255]`. Envía `heat` en el payload.
- **Slider PWM — Ventilador 1:** rango `[0, 255]`. Envía `fans[0]`.
- **Slider PWM — Ventilador 2:** rango `[0, 255]`. Envía `fans[1]`.
- **Switch AC Master:** activa/desactiva el paso de corriente alterna. Requiere confirmación del usuario y dispara feedback háptico (`HapticFeedbackConstants.CONFIRM`).

**Regla de negocio:** en Modo Manual, los campos PID están deshabilitados (visualmente atenuados). La app envía `"op": "manual"` en el payload de control.

### 3.2 Modo Control de Proceso (Lazo Cerrado PID)

El algoritmo PID corre **100% en la ESP32**. La app actúa como panel de configuración y supervisión.

**Parámetros configurables:**
- **Setpoint (SP):** temperatura objetivo en °C. Input numérico validado `Float`, rango `[0.0, 200.0]`.
- **Kp:** constante proporcional. `Float ≥ 0`.
- **Ki:** constante integral. `Float ≥ 0`.
- **Kd:** constante derivativa. `Float ≥ 0`.
- **Tiempo de muestreo (Ts):** periodo de muestreo en ms. `Int`, rango `[50, 5000]`.
- **Tiempo total de sesión (Ttotal):** duración planificada del ensayo en segundos. `Int > 0`. Usado como referencia para el eje X de la gráfica y para el progreso de la sesión.

**Perturbaciones manuales (disponible en Modo Control):**
- El operador puede modificar los sliders de velocidad de los ventiladores en cualquier momento durante el ensayo para introducir perturbaciones externas al sistema y observar la respuesta del controlador. La ESP32 ignorará estos valores para el control térmico interno, pero la app los enviará y los registrará en el log.

> **Nota futura:** se planea que la ESP32 gestione los ventiladores automáticamente como parte del lazo de control. Esta funcionalidad se especificará en una revisión posterior del PRD.

**Regla de negocio:** al cambiar a Modo Control, los sliders de PWM de resistencia se deshabilitan (la ESP32 los controla internamente). La app envía `"op": "set"` con los parámetros PID y el setpoint. La ESP32 confirma el inicio del lazo enviando `"st": 1` en la telemetría.

---

## 4. Requerimientos Funcionales

### RF01 — Gestión de Conectividad BLE

- Escaneo de dispositivos BLE filtrando por nombre del servicio GATT (`THERMAL_CONTROL_SERVICE`).
- Gestión de estados de conexión (ver Sección 7: Máquina de Estados).
- **Auto-reconnect:** si la conexión se pierde y el RSSI del último dispositivo conocido es ≥ -80 dBm, la app intenta reconexión automática con backoff exponencial (1s, 2s, 4s, máx. 30s).
- Indicador visual persistente del estado de conexión en el `ConnectionHeader`.
- El MTU se negocia a **512 bytes** al establecer la conexión para soportar payloads JSON completos sin fragmentación.

### RF02 — Modo Manual: Control Directo de Planta

- Sliders para PWM de resistencia (heat) y ventiladores 1 y 2, rango `[0, 255]`.
- Switch AC Master con confirmación y feedback háptico.
- Validación: no se puede activar el AC si el PWM de resistencia es > 0 sin confirmación explícita del usuario (diálogo de advertencia).
- Los comandos se envían por BLE Write al cambiar cualquier valor (debounce de 150ms para sliders).

### RF03 — Modo Control de Proceso: Configuración PID

- Inputs numéricos validados para: Setpoint, Kp, Ki, Kd, Ts, Ttotal.
- Validación en tiempo real con mensajes de error inline (sin Toasts).
- El botón **"Iniciar Control"** se habilita solo cuando todos los campos son válidos y la conexión es `CONNECTED_READY`.
- El botón **"Detener Control"** envía `"op": "stop"` y retorna al estado `CONNECTED_READY`.
- Durante el control activo, los sliders de ventiladores permanecen habilitados para introducir perturbaciones.

### RF04 — Visualización en Tiempo Real (Vico Charts)

- Implementación de `CartesianChart` de la librería Vico (`com.patrykandpatrick.vico:compose`).
- Eje X: time-series dinámico basado en el campo `"t"` del payload (ms desde inicio de sesión).
- Eje Y: temperatura PT100 en °C.
- En Modo Control se muestra una línea horizontal de referencia para el Setpoint.
- **Freeze (Pausa de Renderizado):** botón que congela la gráfica visualmente sin detener la captura de datos en background. Al descongelar, la gráfica salta al estado actual.
- La captura de datos **nunca se detiene** por cambios de configuración de pantalla (rotación). El buffer vive en el `Repository`.

### RF05 — Persistencia y Exportación de Sesión

- Buffer en memoria (`ArrayList<TelemetryData>`) en el `Repository`.
- Estrategia de **Double Buffering / Flush:**
  - Flush automático al alcanzar 1000 registros en el buffer.
  - Flush manual al presionar "Guardar Sesión" o "Exportar".
  - Escritura asíncrona en `Dispatchers.IO` usando `BufferedWriter`.
- **Formato de archivo:** CSV con encabezado.
- **Nomenclatura:** `LOG_YYYYMMDD_HHMMSS.txt`
- **Ubicación:** `Android/data/com.app.thermalcontrol/files/sessions/`
- **Contenido del CSV:**

```
# SESIÓN: LOG_20250710_143022.txt
# MODO: CONTROL_PROCESO
# SETPOINT: 75.0 °C | Kp: 1.5 | Ki: 0.05 | Kd: 0.1 | Ts: 100ms | Ttotal: 300s
# DISPOSITIVO BLE: ThermalPlant-001
#
timestamp_ms,temperatura_c,pwm_resistencia,pwm_ventilador1,pwm_ventilador2,estado_ac,estado_sistema
0,25.30,0,0,0,false,Run
100,25.35,200,128,60,true,Run
...
```

- En **Modo Manual**, el encabezado PID se omite y se reemplaza por `# MODO: MANUAL`.
- Exportación compartida vía `ShareCompat` (compartir a Drive, WhatsApp, correo, etc.).

---

## 5. Requerimientos No Funcionales

| ID | Categoría | Requisito |
|---|---|---|
| RNF01 | Latencia | Procesamiento del JSON de telemetría en < 10ms en el hilo de parsing. |
| RNF02 | Robustez | `Try-Catch` obligatorio en todo parseo de JSON. Tramas incompletas se descartan con log silencioso. |
| RNF03 | UX | Feedback háptico (`HapticFeedbackConstants.CONFIRM`) al activar/desactivar el Switch AC. |
| RNF04 | UX | Los inputs PID deben mostrar validación inline sin interrumpir el flujo del usuario. |
| RNF05 | Rendimiento | El buffer en memoria no debe impactar el framerate de la UI (escritura siempre en `Dispatchers.IO`). |
| RNF06 | Compatibilidad | Min SDK API 29 (Android 10). Las APIs de BLE usadas (`BluetoothLeScanner`, `BluetoothGatt`) son estables desde API 21. |
| RNF07 | Permisos | Gestión de permisos en runtime: `BLUETOOTH_SCAN`, `BLUETOOTH_CONNECT` (API 31+), `ACCESS_FINE_LOCATION` (API 29-30). |
| RNF08 | Estado | El modo de operación activo (Manual / Control) debe sobrevivir a cambios de configuración de pantalla. |

---

## 6. Protocolo de Mensajería (JSON BLE)

### 6.1 Payload de Telemetría — ESP32 → App (Notify)

```json
{
  "t":      450012,   // Timestamp en ms desde inicio del sistema
  "p100":   38.52,    // Temperatura PT100 en °C
  "st":     1,        // Estado: 0=Stop, 1=Run, 2=Overheat
  "pwm_h":  120       // Feedback del PWM actual de la resistencia
}
```

El parser usará `kotlinx.serialization` con `Json { ignoreUnknownKeys = true }` para tolerar campos experimentales del firmware.

### 6.2 Payload de Control — App → ESP32 (Write)

**Modo Manual:**
```json
{
  "op":   "manual",
  "fans": [128, 60],
  "heat": 200,
  "ac":   true
}
```

**Inicio de Control PID:**
```json
{
  "op":   "set",
  "ac":   true,
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

**Perturbación de ventiladores durante Control PID:**
```json
{
  "op":   "perturb",
  "fans": [200, 200]
}
```

**Detener operación:**
```json
{
  "op": "stop"
}
```

---

## 7. Máquina de Estados de Conectividad BLE

```
                    ┌─────────────────────────────────────────────┐
                    │                                             │
                    ▼                                             │  RSSI aceptable
             DISCONNECTED ──── scan() ────► SCANNING             │  (auto-reconnect)
                    ▲                          │                  │
                    │                     device found           │
              error / timeout                 ▼                  │
                    │                     CONNECTING ────────────┘
                    │                          │
                    │                     GATT connected
                    │                          ▼
                    │                  CONNECTED_READY
                    │                          │
                    │                   start control / manual
                    │                          ▼
                    └──────────── RUNNING (data flow active)
                                               │
                                        stop / error
                                               │
                                               ▼
                                        CONNECTED_READY
```

**Reglas de transición:**
- Los comandos de control solo se envían en estado `CONNECTED_READY` o `RUNNING`.
- En estado `DISCONNECTED`, el `ControlPanel` se deshabilita completamente.
- En estado `SCANNING` o `CONNECTING`, se muestra un `CircularProgressIndicator` en el `ConnectionHeader`.

---

## 8. Mapa de Componentes Compose (Jerarquía)

```
MainActivity (NavHost)
└── MainScreen (Scaffold)
    ├── ConnectionHeader
    │   ├── BleStatusIndicator (color + texto de estado)
    │   ├── DeviceName
    │   └── ConnectDisconnectButton
    │
    ├── LiveChartSection
    │   ├── RealTimeChart (Vico CartesianChart)
    │   ├── SetpointLine (visible en Modo Control)
    │   └── FreezeToggleButton
    │
    ├── ControlPanel (Tab: Manual | Control PID)
    │   ├── [Tab: Manual]
    │   │   ├── PwmSlider (Resistencia heat)
    │   │   ├── PwmSlider (Ventilador 1)
    │   │   ├── PwmSlider (Ventilador 2)
    │   │   └── AcMasterSwitch
    │   │
    │   └── [Tab: Control PID]
    │       ├── PidInputField (Setpoint)
    │       ├── PidInputField (Kp, Ki, Kd) — fila horizontal
    │       ├── PidInputField (Ts, Ttotal) — fila horizontal
    │       ├── PwmSlider (Ventilador 1 — perturbación)
    │       ├── PwmSlider (Ventilador 2 — perturbación)
    │       └── StartStopControlButton
    │
    └── SessionFooter
        ├── RecordingToggleButton (Iniciar / Detener grabación)
        ├── SessionTimer (cronómetro activo durante grabación)
        └── ExportButton (comparte el archivo CSV)
```

---

## 9. Gestión de Dependencias (Version Catalogs)

Archivo: `gradle/libs.versions.toml`

```toml
[versions]
kotlin             = "2.0.0"
compose-bom        = "2024.06.00"
vico               = "1.15.0"
coroutines         = "1.8.1"
koin               = "3.5.6"
serialization      = "1.7.0"
lifecycle          = "2.8.2"

[libraries]
# Compose BOM (gestiona versiones de Compose internamente)
compose-bom        = { group = "androidx.compose", name = "compose-bom", version.ref = "compose-bom" }
compose-ui         = { group = "androidx.compose.ui", name = "ui" }
compose-material3  = { group = "androidx.compose.material3", name = "material3" }

# Gráficas
vico-compose       = { group = "com.patrykandpatrick.vico", name = "compose", version.ref = "vico" }
vico-compose-m3    = { group = "com.patrykandpatrick.vico", name = "compose-m3", version.ref = "vico" }

# Coroutines
coroutines-android = { group = "org.jetbrains.kotlinx", name = "kotlinx-coroutines-android", version.ref = "coroutines" }

# Serialización JSON
serialization-json = { group = "org.jetbrains.kotlinx", name = "kotlinx-serialization-json", version.ref = "serialization" }

# Inyección de Dependencias
koin-android       = { group = "io.insert-koin", name = "koin-android", version.ref = "koin" }
koin-compose       = { group = "io.insert-koin", name = "koin-androidx-compose", version.ref = "koin" }

# Lifecycle / ViewModel
lifecycle-viewmodel-compose = { group = "androidx.lifecycle", name = "lifecycle-viewmodel-compose", version.ref = "lifecycle" }
```

---

## 10. Modelos de Dominio (Data Classes)

```kotlin
// Estado de conexión BLE
enum class ConnectionState { DISCONNECTED, SCANNING, CONNECTING, CONNECTED_READY, RUNNING }

// Modo de operación de la app
enum class AppMode { MANUAL, PROCESS_CONTROL }

// Telemetría recibida de la ESP32
@Serializable
data class TelemetryData(
    @SerialName("t")     val timestampMs: Long,
    @SerialName("p100")  val temperatureC: Float,
    @SerialName("st")    val systemStatus: Int,   // 0=Stop, 1=Run, 2=Overheat
    @SerialName("pwm_h") val heatPwmFeedback: Int
)

// Parámetros del controlador PID
data class PidParameters(
    val setpoint: Float,
    val kp: Float,
    val ki: Float,
    val kd: Float,
    val samplingTimeMs: Int,
    val totalTimeS: Int
)

// Comando enviado a la ESP32
sealed class ControlCommand {
    data class Manual(val heatPwm: Int, val fan1Pwm: Int, val fan2Pwm: Int, val acOn: Boolean) : ControlCommand()
    data class StartPid(val pid: PidParameters, val acOn: Boolean) : ControlCommand()
    data class Perturb(val fan1Pwm: Int, val fan2Pwm: Int) : ControlCommand()
    object Stop : ControlCommand()
}
```

---

## 11. Especificación de Use Cases

### `SendControlCommandUseCase`
- Valida que `heatPwm` ∈ `[0, 255]` y `fanPwm` ∈ `[0, 255]`.
- Valida que `kp`, `ki`, `kd` ≥ 0.
- Serializa el `ControlCommand` a JSON y lo entrega al `BluetoothRepository` para escritura GATT.
- Retorna `Result<Unit>` para manejo de errores en el `ViewModel`.

### `ExportLogUseCase`
- Toma el buffer completo del `Repository` (`List<TelemetryData>`).
- Genera el encabezado CSV con metadatos de sesión (modo, parámetros PID si aplica, dispositivo).
- Escribe en `Dispatchers.IO` usando `BufferedWriter`.
- Retorna el `Uri` del archivo generado para compartir con `ShareCompat`.

### `SetOperationModeUseCase`
- Valida que el estado de conexión sea `CONNECTED_READY` antes de cambiar de modo.
- Si cambia de `PROCESS_CONTROL` a `MANUAL`, envía automáticamente `"op": "stop"` a la ESP32.
- Actualiza el `AppMode` en el `Repository` (fuente de verdad del modo activo).
