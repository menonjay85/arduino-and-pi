#include <esp_now.h>
#include "WiFi.h"

const int relay = 5;

// Structure example to receive data
// Must match the sender structure
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

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Button Pressed?: ");
  Serial.println(myData.d);
  Serial.println("Button State: ");
  Serial.println(myData.bs);
  Serial.println("Reset button: ");
  Serial.println(myData.rst);
  Serial.println("Light State: ");
  Serial.println(myData.ls);
  //Serial.println("Master reset State: ");
  //Serial.println(master_resetPin);
  Serial.println();
  
  if (myData.ls == false){
    //digitalWrite(ledPin, HIGH);
    //digitalWrite(ledPin, myData.ls);
    //digitalWrite(ledPin, myData.ls); //NEW ADD
    digitalWrite(relay, myData.ls);
  }
  
  if (myData.rst == true){
      //digitalWrite(ledPin, myData.ls); //NEW ADD
      digitalWrite(relay, myData.ls);
  }
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, LOW);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  //pinMode(master_resetPin, INPUT);
  //digitalWrite(ledPin, myData.ls); //added
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //--
  /*
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
  */
  //--

  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  /*
  // Set values to send
  myData.m_rst = digitalRead(master_resetPin);

  if (myData.m_rst == true){
    digitalWrite(relay, LOW);
    myData.ls = false;
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
  */
  //myData.m_rst = digitalRead(master_resetPin);

  /*
  if (myData.m_rst == true){
    digitalWrite(relay, LOW);
    if (csStruct[myData.id-1].ls == false){
      csStruct[myData.id-1].ls = true;
      Serial.printf("Hello");
      Serial.println();
    }
  }
  */
}
