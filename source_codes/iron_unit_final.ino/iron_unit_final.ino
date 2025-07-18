#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

/* Receiver’s MAC address (station mode) */
const uint8_t receiverMAC[6] = {0xB4, 0x3A, 0x45, 0xB7, 0xA6, 0xC8};

typedef struct {
  uint32_t packetId;
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float temperature;
} sensor_packet;

sensor_packet dataToSend;
uint32_t packetCounter = 0;

void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  /* ----  MPU‑6050 ---- */
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (true) delay(10);
  }
  Serial.println("MPU6050 initialized");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  delay(100);

  /* ----  ESP‑NOW ---- */
  WiFi.mode(WIFI_STA);          // station‑only mode

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP‑NOW init failed");
    return;
  }
  esp_now_register_send_cb(onSend);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  dataToSend.packetId   = packetCounter++;
  dataToSend.accX       = a.acceleration.x;
  dataToSend.accY       = a.acceleration.y;
  dataToSend.accZ       = a.acceleration.z;
  dataToSend.gyroX      = g.gyro.x;
  dataToSend.gyroY      = g.gyro.y;
  dataToSend.gyroZ      = g.gyro.z;
  dataToSend.temperature = temp.temperature;

  esp_err_t result = esp_now_send(receiverMAC,
                                  reinterpret_cast<uint8_t*>(&dataToSend),
                                  sizeof(dataToSend));

  if (result == ESP_OK) {
    Serial.printf("Packet #%lu sent\n", dataToSend.packetId);
  } else {
    Serial.println("Error sending the data");
  }
  delay(500);
}
