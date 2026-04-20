# PRD - Sistema de Control de Temperatura para Planta Térmica

## 1. Introducción

### 1.1 Objetivo
Desarrollar un sistema de control de temperatura para una planta térmica utilizando un Arduino Nano ESP32. El sistema debe implementar múltiples estrategias de control (PID, RST, identificación) con la capacidad de cambiar dinámicamente entre ellas, manipulat una resistencia térmica mediante modulación de fase (TRIAC) y un ventilador con control PWM.

### 1.2 Alcance
- Control de temperatura entre 18°C y 150°C
- Lectura de sensor PT100
- Modulación de potencia de resistencia mediante TRIAC con detección de cruce por cero
- Control PWM de ventilador para manejo térmico
- Segundo ventilador on/off para perturbaciones
- Sistema de identificación del sistema (respuesta escalón, PRBS)
- Comunicación serial para configuración y monitoreo
- Display LCD 16x2 para visualización de estado

### 1.3 Hardware Objetivo
- **Microcontrolador**: Arduino Nano ESP32
- **Sensor**: PT100 (termorresistencia)
- **Actuadores**:
  - Resistencia controlada por TRIAC (PWM de fase)
  - Ventilador 1: Control PWM (0-100%)
  - Ventilador 2: On/Off (perturbaciones)
  - Relay principal: On/Off general
- **Display**: LCD 16x2
- **Detección**: Detector de cruce por cero

---

## 2. Arquitectura del Sistema

### 2.1 Estructura de Directorios

```
src/
├── main.cpp                          # Entry point
│
├── rtos/                            # Configuración FreeRTOS
│   ├── config/
│   │   └── rtos_config.h            # Prioridades, stacks, timeouts
│   ├── handles.h                    # Handles de tareas, colas, mutexes
│   └── init.cpp                     # Inicialización RTOS
│
├── control/                         # Algoritmos de control
│   ├── pid.cpp/h                    # Algoritmo PID
│   ├── rst.cpp/h                    # Algoritmo RST
│   ├── tuner.cpp/h                  # Ziegler-Nichols, Cohen-Coon
│   └── types.h                      # Enums, estructuras
│
├── controllers/                     # Ejecución de controladores
│   ├── controller_manager.cpp/h    # Gestor - elige controlador
│   ├── pid_executor.cpp/h          # Ejecuta PID
│   └── rst_executor.cpp/h          # Ejecuta RST
│
├── sensors/                         # Lectura de sensores
│   ├── pt100.cpp/h                  # Driver PT100
│   └── sensor_task.cpp/h           # Tarea: leer sensores
│
├── actuation/                       # Actuadores
│   ├── triac.cpp/h                  # Driver TRIAC + cruce por cero
│   ├── fan.cpp/h                    # Driver ventiladores
│   └── actuator_task.cpp/h         # Tarea: actualizar actuadores
│
├── identification/                   # Identificación del sistema
│   ├── prbs.cpp/h                   # Señal PRBS
│   ├── step_response.cpp/h         # Respuesta escalón
│   ├── system_model.cpp/h          # Modelo matemático
│   └── identification_task.cpp/h   # Tarea: identificación
│
├── communication/                   # Comunicación
│   ├── serial.cpp/h                # Protocolo serial
│   └── comm_task.cpp/h             # Tarea: comunicación
│
├── display/                          # Display LCD
│   ├── lcd.cpp/h                    # Driver LCD
│   └── display_task.cpp/h          # Tarea: display
│
└── common/
    ├── data/
    │   └── central_data.h          # Estructura centralizada (única fuente de verdad)
    └── pins/
        └── pins.h                   # Definición de pines
```

### 2.2 Tareas FreeRTOS

| Tarea | Responsabilidad | Prioridad | Frecuencia |
|-------|-----------------|-----------|-------------|
| `sensor_task` | Leer PT100, filtrar datos | tskIDLE_PRIORITY + 3 | 100ms |
| `actuator_task` | Actualizar TRIAC, ventiladores | tskIDLE_PRIORITY + 3 | 50ms |
| `control_task` | Ejecutar PID/RST | tskIDLE_PRIORITY + 2 | 100ms-500ms |
| `identification_task` | Identificación del sistema | tskIDLE_PRIORITY + 1 | Variable |
| `comm_task` | Comunicación serial | tskIDLE_PRIORITY + 1 | On-demand |
| `display_task` | Actualizar LCD 16x2 | tskIDLE_PRIORITY + 0 | 500ms |

### 2.3 Comunicación Inter-Tareas

```
┌─────────────────────────────────────────────────────────────────────┐
│                        FLUJO DE DATOS                                │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│   SENSORES (lectura) ──cola(q_sensor)──► CONTROL (algoritmo)      │
│                                                     │               │
│                                                     ▼               │
│                          ┌──────────────────────────────┐          │
│                          │     CONTROLLER MANAGER        │          │
│                          │  (decide: PID/RST/Ident/Off)  │          │
│                          └──────────────────────────────┘          │
│                                                     │               │
│                                                     ▼               │
│   ACTUADORES ◄──cola(q_control)─── CONTROL (salida)              │
│                                                                     │
│   ════════════════════════════════════════════════════════════    │
│                                                                     │
│   SERIAL ◄──cola(q_serial)─── TODAS LAS TAREAS                    │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 3. Funcionalidad por Módulo

### 3.1 Sensores

#### PT100
- **Rango**: -40°C a 200°C (rango operativo 18-150°C)
- **Lectura**: ADC del ESP32
- **Filtrado**: Promedio móvil simple
- **Frecuencia de muestreo**: 100ms (configurable)
- **Salida**: Temperatura en formato raw (×100) e interpretación en °C

### 3.2 Actuadores

#### TRIAC + Detector de Cruce por Cero
- **Frecuencia de red**: 50Hz (período 20ms, semiperíodo 10ms)
- **Control**: Modulación de fase (phase firing)
- **Ciclo de trabajo**: 0-100% mapeado a delay de 0-10ms
- **Timing crítico**: Disparo del TRIAC debe sincronizarse con cruce por cero
- **Seguridad**: Corte instantáneo ante sobretemperatura

#### Ventilador 1 (Control)
- **Tipo**: PWM variable
- **Rango**: 0-100%
- **Frecuencia PWM**: 25kHz (típico para fans)
- **Uso**: Enfriamiento activo

#### Ventilador 2 (Perturbaciones)
- **Tipo**: On/Off
- **Uso**: Generación de perturbaciones para análisis

#### Relay Principal
- **Tipo**: On/Off
- **Uso**: Corte general de potencia a la resistencia

### 3.3 Controladores

#### Modo MANUAL
- El usuario establece directamente los valores de:
  - Potencia de resistencia (0-100%)
  - Velocidad ventilador 1 (0-100%)
  - Estado ventilador 2 (on/off)
  - Estado relay (on/off)

#### Modo PID
- Implementación clásica: `u(t) = Kp·e(t) + Ki·∫e(t)dt + Kd·de(t)/dt`
- Parámetros:
  - Kp (proporcional)
  - Ki (integral)
  - Kd (derivativa)
- Tuning:
  - **Ziegler-Nichols**: Método de lazo cerrado (Kp crítico, período crítico)
  - **Cohen-Coon**: Método de respuesta escalón (requiere identificación)
- Configuración manual en código

#### Modo RST
- Control de tres términos: R(z⁻¹), S(z⁻¹), T(z⁻¹)
- Diseño basado en modelo del sistema
- Más robusto que PID para sistemas con retardo
- Coeficientes configurables en código

#### Modo OFF
- Sistema en espera
- Todos los actuadores en estado seguro (resistencia apagada, ventiladores máximos)

### 3.4 Identificación del Sistema

#### Métodos implementados:
1. **Respuesta Escalón (Step Response)**
   - Aplicar cambio escalón en la potencia (ej: 0% → 50%)
   - Registrar curva de temperatura
   - Extraer parámetros: K (ganancia), τ (constante de tiempo), θ (retraso)

2. **PRBS (Pseudo-Random Binary Sequence)**
   - Señal binaria pseudo-aleatoria
   - Excita múltiples frecuencias
   - Algoritmo de identificación por mínimos cuadrados
   - Muestras configurables (placeholder: 10-20, modificable)

#### Modelo del Sistema
- **Estructura**: Primer orden con retardo
```
G(s) = K × e^(-θs) / (τs + 1)
```
- K: Ganancia estática
- τ: Constante de tiempo
- θ: Retardo de transporte

#### Uso del Modelo
- El modelo obtenido se utiliza para:
  - Calcular parámetros PID automáticamente (Ziegler-Nichols, Cohen-Coon)
  - Diseñar controlador RST
  - Predicción de comportamiento

### 3.5 Comunicación Serial

#### Protocolo (v1.0 - placeholder para expansión)
- **Baudrate**: 115200
- **Formato**: Texto legible
- **Comandos**: [POR DEFINIR]

#### Funcionalidades:
- Lectura de estado actual (temperatura, setpoint, salida control)
- Cambio de modo (manual, PID, RST, off)
- Cambio de setpoint
- Inicio de identificación
- Consulta de parámetros

#### Mensajes de salida:
- Estado del sistema periódicamente
- Respuesta a comandos
- Errores y advertencias

### 3.6 Display LCD 16x2

#### Información mostrada:
- Línea 1: Estado (MANUAL / PID / RST / OFF) + Setpoint
- Línea 2: Temperatura actual + Potencia aplicada

#### Frecuencia de actualización: 500ms

---

## 4. Seguridad

### 4.1 Límites de Temperatura
- **Temperatura máxima**: 150°C (hard limit)
- **Temperatura mínima**: 18°C

### 4.2 Acción ante SobreTemperatura (>150°C)
1. Cortar PWM de la resistencia (0%)
2. Poner ambos ventiladores a 100%
3. Cortar relay principal (off)
4. Cambiar a modo OFF
5. Enviar mensaje de error por serial
6. Mostrar "ERR" en display

### 4.3 Estados de Emergencia
- Sobre temperatura: Ver sección 4.2
- Falla de sensor: Usar último valor válido, avisar por serial

---

## 5. Datos del Sistema (Estructura Centralizada)

### 5.1 Placeholder - Definición Progresiva

La estructura `central_data` se definirá de forma progresiva durante la implementación:

```cpp
// common/data/central_data.h
// Se ira completando conforme se necesiten los datos

typedef struct {
    // SENSORES - Sección 3.1
    // TODO: Añadir datos del PT100
    
    // ACTUADORES - Sección 3.2
    // TODO: Añadir estados de TRIAC, ventiladores, relay
    
    // CONTROL - Sección 3.3
    // TODO: Añadir modo actual, setpoint, parámetros PID/RST
    
    // IDENTIFICACIÓN - Sección 3.4
    // TODO: Añadir estado de identificación, modelo
    
    // SISTEMA
    // TODO: Añadir flags, errores
} central_data_t;

extern central_data_t g_data;
```

---

## 6. Pendientes por Definir

### Alta Prioridad
- [ ] Protocolo serial detallado (comandos específicos)
- [ ] Formato de mensajes de comunicación

### Media Prioridad
- [ ] Detalle de visualización en LCD
- [ ] Frecuencias exactas de muestreo
- [ ] Algoritmo de filtrado del sensor

### Baja Prioridad
- [ ] Expansión a Bluetooth
- [ ] Interfaz de usuario (botones, potenciómetros)
- [ ] Guardado de configuración en EEPROM

---

## 7. Restricciones Técnicas

### Hardware
- Arduino Nano ESP32
- RAM limitada (~320KB)
- Flash 4MB
- Tensiones: 3.3V (ESP32), 5V (LCD, Relay)

### Timing
- Detección de cruce por cero: <1ms de precisión
- Control PID: 100-500ms (depende de dinámica térmica)
- Display: 500ms (baja prioridad)

### Memoria
- Usar tipos mínimos (int8_t, uint16_t, etc.)
- Stack calculado para cada tarea
-Buffers de identificación: Configurables

---

## 8. Bibliografía de Referencia

- Ziegler, J.G., Nichols, N.B. (1942). "Optimum Settings for Automatic Controllers"
- Cohen, G.H., Coon, G.A. (1953). "Theoretical Consideration of Retarded Control"
- Åström, K.J., Hägglund, T. (1995). "PID Controllers: Theory, Design, and Tuning"
- Franklin, G.F., et al. (2015). "Feedback Control of Dynamic Systems"

---

## 9. Glosario

| Término | Definición |
|---------|------------|
| PT100 | Sensor de temperatura tipo termorresistencia (100Ω a 0°C) |
| TRIAC | Semiconductor para control de potencia AC |
| Cruce por cero | Momento donde la onda AC cruisa 0V |
| PID | Control Proporcional-Integral-Derivativo |
| RST | Control con tres polinomios (Reference, Sensitivity, Tracking) |
| PRBS | Pseudo-Random Binary Sequence |
| Setpoint | Valor de consigna (temperatura objetivo) |
| PWM | Pulse Width Modulation |
| RTOS | Real-Time Operating System |

---

## 10. Historial de Versiones

| Versión | Fecha | Descripción |
|---------|-------|-------------|
| 1.0 | 2026-04-19 | Versión inicial del PRD |

---

*Este documento es un living document y se actualizará conforme avance el desarrollo.*
