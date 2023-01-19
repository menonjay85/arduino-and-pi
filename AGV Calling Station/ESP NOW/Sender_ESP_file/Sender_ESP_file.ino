#include <esp_now.h>
#include "WiFi.h"

const int buttonPin = 34;

const int resetPin = 35; //NEW ADD

//unsigned long lastDebounceTime = 0;  //added
//unsigned long debounceDelay = 50;    //added

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x8E, 0xE6, 0x0C};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
  bool lbs; //added
  bool bs; //added
  bool ls = false; //added
  unsigned long lastDebounceTime = 0; //added
  unsigned long debounceDelay = 50; //added

  bool rst; //NEW ADD
} struct_message;

// Create a struct_message called myData
struct_message myData;


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

  pinMode(resetPin, INPUT); //NEW ADD
  

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
}
 
void loop() {
  // Set values to send
  myData.b = random(1,20); //ignore
  myData.c = 1.2; //ignore
  myData.d = digitalRead(buttonPin); 
  myData.rst = digitalRead(resetPin);
  //myData.d = true;

  //added
  if (myData.d != myData.bs){
    myData.bs = myData.d;
    if (myData.bs == true) {
      //myData.ls = !myData.ls;
      myData.ls = true; //NEW ADD
    }
    
    if (myData.rst == true && myData.ls == true){
      myData.ls = false; //NEW ADD
    }
  }
  //added
  
  if (myData.d == false){
    strcpy(myData.a, "THIS IS A CHAR");
  }
  if (myData.rst == true){
    strcpy(myData.a, "Successfully RESET"); //NEW ADD
  }
  else {
    strcpy(myData.a, "IT WORKS");
    //if (myData.rst == true && myData.ls == true){
      //strcpy(myData.a, "Successfully RESET"); //NEW ADD
    //}
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
