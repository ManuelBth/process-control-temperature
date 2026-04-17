# Protocolo de Comunicación: Thermal Control System
**Versión:** 2.0.0
**Transporte:** Bluetooth Low Energy (BLE)
**Formato de Carga:** JSON (Minified)
**MTU Target:** 128 bytes (Requerido)

---

## 1. Estrategia de Comunicación

Para optimizar el throughput en BLE se utilizan identificadores de clave cortos. El diseño separa los mensajes por responsabilidad: telemetría, cambio de modo, control manual y configuración PID. Esto permite parseo selectivo y reduce el acoplamiento entre capas.

| Dirección | Mensaje | Característica GATT | Frecuencia |
| :--- | :--- | :--- | :--- |
| ESP32 → App | `MSG_TELEMETRY` | `NOTIFY` | 1 Hz (paquete de 10 muestras) |
| App → ESP32 | `MSG_MODE` | `WRITE_NO_RESPONSE` | Por evento (cambio de modo) |
| App → ESP32 | `MSG_CONTROL` | `WRITE_NO_RESPONSE` | Por evento · Solo modo Manual |
| App → ESP32 | `MSG_PID_CFG` | `WRITE_NO_RESPONSE` | Por evento · Cualquier modo |

---

## 2. Telemetría — `MSG_TELEMETRY` (ESP32 → App)

**Frecuencia:** 1 Hz — 10 muestras por paquete (muestreo interno a 10 Hz).
**GATT Characteristic:** `NOTIFY`

La App reconstruye la serie temporal de cada muestra con la fórmula:

```
timestamp[i] = t0 + (i × 100)   // ms
```

### Campos

| Clave | Campo | Tipo | Rango / Unidad | Descripción |
| :--- | :--- | :--- | :--- | :--- |
| `id` | Packet ID | Int | 0 – 65535 (rollover) | Contador incremental. Permite detectar paquetes perdidos en la App. |
| `t0` | Timestamp base | Long | ms desde boot | Tiempo del primer sample del array. |
| `v` | Temperature samples | Float[10] | °C, 1 decimal | Array de 10 lecturas del sensor PT100 a 10 Hz. |
| `st` | System status | Int | [0, 1, 2, 3] | Ver tabla de estados. |
| `f1` | Fan 1 feedback | Int | 0 – 255 PWM | Valor PWM actual del ventilador de entrada. |
| `f2` | Fan 2 feedback | Int | 0 – 255 PWM | Valor PWM actual del ventilador de salida. |
| `r` | Heater feedback | Int | 0 – 255 PWM | Valor PWM actual de la resistencia calefactora. |
| `ac` | AC state | Int | [0, 1] | Estado real del corte AC. `0` = cortado, `1` = energizado. |

### Estados del sistema (`st`)

| Valor | Estado | Descripción |
| :--- | :--- | :--- |
| `0` | Idle / Standby | Sistema detenido, actuadores en reposo. |
| `1` | Running · Manual | En operación bajo control manual desde la App. |
| `2` | Running · PID | En operación bajo control automático PID. |
| `3` | Alarm / Error | Fallo detectado. Requiere intervención. |

### Ejemplo de payload (~110 bytes)

```json
{"id":42,"t0":120000,"v":[45.8,46.0,46.1,46.3,46.4,46.5,46.5,46.6,46.7,46.8],"st":2,"f1":180,"f2":120,"r":200,"ac":1}
```

---

## 3. Cambio de Modo — `MSG_MODE` (App → ESP32)

**Frecuencia:** Por evento (cambio de modo).
**GATT Characteristic:** `WRITE_NO_RESPONSE`

### Campos

| Clave | Campo | Tipo | Rango | Descripción |
| :--- | :--- | :--- | :--- | :--- |
| `cmd` | Command | Int | [0, 1, 2] | Ver tabla de comandos. |

### Comandos (`cmd`)

| Valor | Comando | Descripción |
| :--- | :--- | :--- |
| `0` | STOP | Detiene el proceso. Fuerza `ac:0` y `r:0` por seguridad térmica. |
| `1` | START\_MANUAL | Activa modo control manual desde la App. |
| `2` | START\_PID | Activa modo control automático PID. |

> **Nota:** Al recibir `cmd:2`, la ESP32 ignora los campos de actuadores de `MSG_CONTROL` hasta que se cambie el modo. El PID toma el control exclusivo de `r`, `f1` y `f2`.

### Ejemplos de payload

```json
{"cmd":0}
{"cmd":1}
{"cmd":2}
```

---

## 4. Control Manual — `MSG_CONTROL` (App → ESP32)

**Frecuencia:** Por evento.
**GATT Characteristic:** `WRITE_NO_RESPONSE`
**Condición:** Solo efectivo cuando el sistema está en modo `START_MANUAL` (`cmd:1`).

### Campos

| Clave | Campo | Tipo | Rango / Unidad | Descripción |
| :--- | :--- | :--- | :--- | :--- |
| `ac` | AC power | Int | [0, 1] | Corte físico de corriente AC. Debe enviarse `1` antes de activar actuadores térmicos. |
| `f1` | Fan 1 setpoint | Int | 0 – 255 PWM | Consigna PWM del ventilador de entrada. `0` = apagado. |
| `f2` | Fan 2 setpoint | Int | 0 – 255 PWM | Consigna PWM del ventilador de salida. `0` = apagado. |
| `r` | Heater setpoint | Int | 0 – 255 PWM | Consigna PWM de la resistencia calefactora. Solo efectivo si `ac:1`. |

### Ejemplo de payload

```json
{"ac":1,"f1":180,"f2":120,"r":200}
```

---

## 5. Configuración PID — `MSG_PID_CFG` (App → ESP32)

**Frecuencia:** Por evento.
**GATT Characteristic:** `WRITE_NO_RESPONSE`
**Condición:** Válido en cualquier modo. Los parámetros se aplican en el siguiente ciclo de control.

### Campos

| Clave | Campo | Tipo | Rango / Unidad | Descripción |
| :--- | :--- | :--- | :--- | :--- |
| `sp` | Setpoint | Float | °C | Temperatura objetivo del proceso para el controlador PID. |
| `p` | Kp | Float | N/A | Constante proporcional del PID. |
| `i` | Ki | Float | N/A | Constante integral del PID. |
| `d` | Kd | Float | N/A | Constante derivativa del PID. |

> **Nota:** Los parámetros PID se pueden actualizar en caliente sin detener el proceso.

### Ejemplo de payload

```json
{"sp":65.0,"p":1.5,"i":0.05,"d":0.1}
```

---

## 6. Gestión de Errores y Failsafe

### 6.1 Watchdog de Comunicación

Si la ESP32 no recibe ningún mensaje de la App en **5 segundos**, ejecuta la secuencia de failsafe:

1. Fuerza `ac:0` y `r:0`.
2. Transiciona a `st:3` (Alarm).
3. Espera reconexión BLE y un nuevo comando de modo.

### 6.2 Latido de Seguridad (Heartbeat)

Durante operación activa (`cmd:1` o `cmd:2`), la App debe reenviar el comando de modo activo cada **1 segundo** para alimentar el watchdog. Si el latido cesa, se activa el failsafe del punto 6.1.

```json
{"cmd":2}   // heartbeat en modo PID
```

### 6.3 Lectura de Sensor Inválida

Si el sensor PT100 reporta un valor fuera de rango o NaN:

- La muestra afectada se rellena con `null` en el array `v`.
- Se incrementa un contador interno de errores.
- A **3 errores consecutivos**, la ESP32 transiciona a `st:3` y activa el failsafe.

### 6.4 Buffer Overflow en App

La App implementará un `SharedFlow` con estrategia `DROP_OLDEST` para evitar saturación de UI si el flujo de datos supera la tasa de refresco de la interfaz.

---

## 7. Resumen de Claves

| Clave | Contexto | Significado |
| :--- | :--- | :--- |
| `id` | TELEMETRY | Packet ID |
| `t0` | TELEMETRY | Timestamp base del array |
| `v` | TELEMETRY | Array de temperaturas |
| `st` | TELEMETRY | Estado del sistema |
| `f1` | TELEMETRY / CONTROL | Fan 1 (feedback / setpoint) |
| `f2` | TELEMETRY / CONTROL | Fan 2 (feedback / setpoint) |
| `r` | TELEMETRY / CONTROL | Heater (feedback / setpoint) |
| `ac` | TELEMETRY / CONTROL | Estado / corte AC |
| `cmd` | MODE | Comando de modo |
| `sp` | PID\_CFG | Setpoint de temperatura |
| `p` | PID\_CFG | Kp |
| `i` | PID\_CFG | Ki |
| `d` | PID\_CFG | Kd |
