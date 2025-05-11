#include <NimBLEDevice.h>

void setup(){
    NimBLEDevice::init("ESP32_NimBLE");
    NimBLEServer* pServer = NimBLEDevice::createServer();
    NimBLEService* pService = pServer->createService("12345678-1234-1234-1234-123456789abc");

    pService ->createCharacteristic(
        "87654321-4321-4321-4321-cba987654321",
        NIMBLE_PROPERTY::NOTIFY
    );
    pService->start();
    

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();

    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->setName("ESP32_NimBLE"); // Set the name of the device
    pAdvertising->start();
    Serial.println("Waiting for a client connection...");
}

void loop(){}