#include <NimBLEDevice.h>

NimBLEServer* pServer = nullptr;
NimBLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;
uint32_t value = 0;

class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(NimBLEServer* pServer) {
        deviceConnected = false;
    }
};

void setup() {
    Serial.begin(115200);
    NimBLEDevice::init("ESP32_NimBLE");
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    NimBLEService* pService = pServer->createService("12345678-1234-1234-1234-123456789abc");
    pCharacteristic = pService->createCharacteristic(
        "87654321-4321-4321-4321-cba987654321",
        NIMBLE_PROPERTY::NOTIFY
    );

    pService->start();
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();
    Serial.println("Waiting for a client connection...");
}

void loop() {
    if (deviceConnected) {
        value++;
        pCharacteristic->setValue(value);
        pCharacteristic->notify();
        Serial.println("Notification sent: " + String(value));
        delay(500); // Half a second delay
    }
}