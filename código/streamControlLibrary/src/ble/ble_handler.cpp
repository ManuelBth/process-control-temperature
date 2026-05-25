#include "ble_handler.h"
#include "ble_config.h"
#include "message_types.h"
#include "message_parser.h"
#include "NimBLEDevice.h"

// ============================================
// BLE Server Globals
// ============================================
NimBLEServer* pServer = nullptr;
NimBLEService* pService = nullptr;
NimBLECharacteristic* pTxChar = nullptr;  // FFF1 - receives from app (WRITE)
NimBLECharacteristic* pRxChar = nullptr;  // FFF2 - sends to app (NOTIFY)

// ============================================
// Advertising Restart Flag
// ============================================
static volatile bool g_needs_advertising_restart = false;

// ============================================
// Server Callbacks - Connection Events
// ============================================
void ServerCallbacks::onConnect(NimBLEServer* pServer) {
    Serial.println("[BLE] Client connected");
    g_needs_advertising_restart = false;
}

void ServerCallbacks::onDisconnect(NimBLEServer* pServer) {
    Serial.println("[BLE] Client disconnected");
    g_needs_advertising_restart = true;
}

// ============================================
// Characteristic Callbacks - Data Received from App
// ============================================
void MyCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    NimBLEAttValue value = pCharacteristic->getValue();
    const uint8_t* data = value.data();
    uint16_t length = value.length();

    if (length > 0 && data != nullptr) {
        // Parse the incoming JSON message
        char* json_buffer = (char*)malloc(length + 1);
        if (json_buffer != nullptr) {
            memcpy(json_buffer, data, length);
            json_buffer[length] = '\0';
            message_parser_parse(json_buffer, length);
            free(json_buffer);
        }
    }
}

// ============================================
// BLE Initialization
// ============================================
void ble_init() {
    // Initialize NimBLE
    NimBLEDevice::init(BLE_DEVICE_NAME);

    // Create server and service
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    pService = pServer->createService(BLE_SERVICE_UUID);

    // TX Characteristic (FFF1) - receives from app
    pTxChar = pService->createCharacteristic(
        BLE_CHAR_RX_UUID,
        NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR
    );
    pTxChar->setCallbacks(new MyCallbacks());

    // RX Characteristic (FFF2) - sends to app
    pRxChar = pService->createCharacteristic(
        BLE_CHAR_TX_UUID,
        NIMBLE_PROPERTY::NOTIFY
    );

    pService->start();

    // Start advertising
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName(BLE_DEVICE_NAME);
    pAdvertising->addServiceUUID(NimBLEUUID(BLE_SERVICE_UUID));
    NimBLEDevice::startAdvertising();

    Serial.println("[BLE] Advertising started - Waiting for connection");

    message_parser_init();
}

// ============================================
// BLE Task - runs in RTOS task context
// ============================================
void ble_task(void* params) {
    ble_init();
    vTaskDelete(NULL);
}

// ============================================
// Start BLE (deferred to RTOS task)
// ============================================
void ble_init_deferred() {
    xTaskCreate(ble_task, "BLE", 4096, NULL, 3, NULL);
}

// ============================================
// Restart Advertising (from main loop)
// ============================================
void ble_restart_advertising_from_loop() {
    if (g_needs_advertising_restart) {
        NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
        pAdvertising->start();
        g_needs_advertising_restart = false;
    }
}

// ============================================
// Send Notification to App
// ============================================
void ble_send_notification(const uint8_t* data, size_t length) {
    if (pRxChar == nullptr || pRxChar->getSubscribedCount() == 0) {
        return;
    }

    NimBLEAttValue value;
    value.setValue(data, length);
    pRxChar->notify(value, true);
}