#ifndef BLE_HANDLER_H
#define BLE_HANDLER_H

#include <Arduino.h>
#include <NimBLEServer.h>

// Forward declaration
class NimBLEServer;

// ============================================
// BLE Server Callback Class
// ============================================
class ServerCallbacks : public NimBLEServerCallbacks {
public:
    void onConnect(NimBLEServer* pServer) override;
    void onDisconnect(NimBLEServer* pServer) override;
};

// ============================================
// BLE Characteristic Callback Class
// ============================================
class MyCallbacks : public NimBLECharacteristicCallbacks {
public:
    void onWrite(NimBLECharacteristic* pCharacteristic) override;
};

// ============================================
// BLE Function Declarations
// ============================================
void ble_init();
void ble_init_deferred();
void ble_restart_advertising_from_loop();
void ble_send_notification(const uint8_t* data, size_t length);

#endif // BLE_HANDLER_H