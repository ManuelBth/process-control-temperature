// ============================================================================
// FreeRTOS Configuration - ESP32-specific
// ============================================================================

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// ============================================================================
// REQUIRED DEFINITIONS - Must be defined
// ============================================================================

// Maximum number of priorities
#define configMAX_PRIORITIES 24

// Use preemptive scheduling
#define configUSE_PREEMPTION 1

// Idle hook
#define configUSE_IDLE_HOOK 1

// Tick hook
#define configUSE_TICK_HOOK 0

// 16-bit ticks
#define configUSE_16_BIT_TICKS 0

// Tick rate in Hz
#define configTICK_RATE_HZ 1000

// Minimal stack size (words)
#define configMINIMAL_STACK_SIZE 256

// Task name length
#define configMAX_TASK_NAME_LEN 16

// Use static allocation
#define configSUPPORT_STATIC_ALLOCATION 1

// Enable mutexes
#define configUSE_MUTEXES 1

// Enable recursive mutexes
#define configUSE_RECURSIVE_MUTEXES 1

// Use counting semaphores
#define configUSE_COUNTING_SEMAPHORES 1

// Queue sets enabled
#define configUSE_QUEUE_SETS 0

// Timer task priority
#define configTIMER_TASK_PRIORITY (tskIDLE_PRIORITY + 3)

// Timer stack size (words)
#define configTIMER_STACK_LENGTH 256

// Stack overflow check method (1 = check both)
#define configCHECK_FOR_STACK_OVERFLOW 2

// Heap size for Arduino (leave to Arduino core)
#define configTOTAL_HEAP_SIZE 0

#endif // FREERTOS_CONFIG_H