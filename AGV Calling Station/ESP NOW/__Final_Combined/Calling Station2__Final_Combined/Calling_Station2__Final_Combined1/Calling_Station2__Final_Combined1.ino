#include <esp_now.h>
#include "WiFi.h"

const int buttonPin = 35;
const int resetPin = 34;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x3C, 0x61, 0x05, 0x15, 0x45, 0x34};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int id;
  char a[32];
  bool d;
  bool lbs;
  bool bs;
  bool ls;
  bool rst;
  bool m_rst; 
} struct_message;

// Create a struct_message called myData
struct_message myData;

//-------
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Master Reset?: ");
  Serial.println(myData.m_rst);
  Serial.println();
}
//-------

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

  pinMode(buttonPin, INPUT);
  myData.bs = false;
  pinMode(resetPin, INPUT);
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Set values to send
  myData.id = 2;
  myData.d = digitalRead(buttonPin); 
  myData.rst = digitalRead(resetPin);

  if (myData.d != myData.bs){
    myData.bs = myData.d;
    if (myData.bs == true) {
      //myData.ls = !myData.ls; (Only if we are using 1 button)
      strcpy(myData.a, "Called!");
      myData.ls = false;
      Serial.println();
      Serial.printf("press if part: ");
      Serial.print(myData.ls);
      Serial.println();      
    }
    else {
      strcpy(myData.a, "Have not called yet.");
      myData.bs = myData.d;
      Serial.println();
      Serial.printf("press else part: ");
      Serial.print(myData.ls);
      Serial.println();
    }
  }

  if (myData.rst == true){
    strcpy(myData.a, "Successfully RESET");
    myData.ls = true;
    Serial.println();
    Serial.printf("reset else part: ", myData.ls);
    Serial.println();
  }

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
