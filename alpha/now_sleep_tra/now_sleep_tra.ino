/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_sleep.h>

// REPLACE WITH YOUR RECEIVER MAC Address
// uint8_t broadcastAddress[] = {0xec, 0xe3, 0x34, 0x7b, 0xe5, 0x14};
uint8_t broadcastAddress[] = {0xec, 0xe3, 0x34, 0xb4, 0x9f, 0xe0};
// uint8_t broadcastAddress_all[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

//sleep time
uint64_t sleepTime = 2000000; //2 seconds

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  float acclr;
  int packetCount;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

int i = 0;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  esp_sleep_enable_timer_wakeup(sleepTime);  
}
 
void loop() {
  // Set values to send
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  i++;
  strcpy(myData.a, "Sasith");
  myData.acclr = i;
  myData.packetCount = i;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData)); 
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  Serial.println(i);
  delay(500);

  Serial.println("Sleeping");
  delay(100);
  esp_wifi_stop();
  esp_light_sleep_start();
  // Serial.println("woke up");
  delay(100);
  esp_wifi_start();
}