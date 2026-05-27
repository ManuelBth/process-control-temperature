#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

// ============================================
// BLE Message Types
// ============================================
// These must match the app's message types

#define MESSAGE_TYPE_CONFIG_SYNC   "config_sync"
#define MESSAGE_TYPE_START_CONTROL "start_control"
#define MESSAGE_TYPE_STOP_CONTROL  "stop_control"
#define MESSAGE_TYPE_CONTROL_DATA  "control_data"

// ============================================
// JSON Field Names
// ============================================
#define JSON_FIELD_TYPE       "type"
#define JSON_FIELD_SEQUENCE   "sequence"
#define JSON_FIELD_SAMPLES    "samples"
#define JSON_FIELD_TIME       "t"
#define JSON_FIELD_TEMP       "temp"
#define JSON_FIELD_ANGLE      "angle"
#define JSON_FIELD_PWM        "pwm"

// ============================================
// Config Fields (for config_sync message)
// ============================================
#define JSON_FIELD_SAMPLE_INTERVAL "sample_interval_ms"
#define JSON_FIELD_CONTROL_MODE    "control_mode"
#define JSON_FIELD_SETPOINT        "setpoint"
#define JSON_FIELD_KP              "kp"
#define JSON_FIELD_KI              "ki"
#define JSON_FIELD_KD              "kd"

#endif // MESSAGE_TYPES_H