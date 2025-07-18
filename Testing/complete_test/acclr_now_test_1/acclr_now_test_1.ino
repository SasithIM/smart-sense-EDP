//libraries for MPU6050
#include <Wire.h>
#include <MPU6050.h>

//initialize the MPU6050
MPU6050 mpu;

//libraries for ESP-NOW
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
// uint8_t broadcastAddress[] = {0xec, 0xe3, 0x34, 0x7b, 0xe5, 0x14};
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};;

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[16];
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
 
//packet counter
int packetCount = 0;

// MPU6050 variables
int16_t ax, ay, az;
int16_t gx, gy, gz;


void setup(){
    //Serial
    Serial.begin(115200);

    //setup mpu6050
    Wire.begin();
    mpu.initialize();

    if (mpu.testConnection()) {
        Serial.println("MPU6050 connection successful.");
    } else {
        Serial.println("MPU6050 connection failed.");
    }

    //setup ESP-NOW
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
  }
}

void loop(){
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int acclr_raw = (abs(ax)+abs(ay))/2;
  Serial.println((acclr_raw));
  String acclr = ((abs(ax)+abs(ay))/2 > 200) ? "yes":"no";
  // Serial.println(acclr);

  //Send the data
  strcpy(myData.a, acclr.c_str());
  myData.acclr = acclr_raw;
  myData.packetCount = i;
  i++;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}