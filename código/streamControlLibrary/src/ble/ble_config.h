#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

#include <stdint.h>

// ============================================
// BLE Service and Characteristic UUIDs
// ============================================

// BLE Service UUID: 0000FFF0-0000-1000-8000-00805F9B34FB
#define BLE_SERVICE_UUID         "0000FFF0-0000-1000-8000-00805F9B34FB"

// BLE Characteristic UUID for RX (ESP receives from app): 0000FFF1-...
#define BLE_CHAR_RX_UUID         "0000FFF1-0000-1000-8000-00805F9B34FB"

// BLE Characteristic UUID for TX (ESP sends to app): 0000FFF2-...
#define BLE_CHAR_TX_UUID         "0000FFF2-0000-1000-8000-00805F9B34FB"

// ============================================
// BLE Device Name
// ============================================
#define BLE_DEVICE_NAME          "StreamControl"

// ============================================
// Message Type Constants
// ============================================
#define MSG_TYPE_CONFIG_SYNC     "config_sync"
#define MSG_TYPE_START_CONTROL   "start_control"
#define MSG_TYPE_STOP_CONTROL    "stop_control"
#define MSG_TYPE_CONTROL_DATA    "control_data"

// ============================================
// BLE Configuration Constants
// ============================================
#define BLE_ADVERTISING_INTERVAL_MS   100
#define BLE_CONN_INTERVAL_MIN          6    // * 1.25ms = 7.5ms
#define BLE_CONN_INTERVAL_MAX          8    // * 1.25ms = 10ms
#define BLE_CONN_SLAVE_LATENCY         0
#define BLE_CONN_TIMEOUT_MS            500  // * 10ms = 5000ms

// ============================================
// Default Sample Interval (ms)
// ============================================
#define DEFAULT_SAMPLE_INTERVAL_MS     100

#endif // BLE_CONFIG_H