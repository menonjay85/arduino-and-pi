#include <esp_now.h>
#include "WiFi.h"

const int relay = 5;
const int relay2 = 4;
const int relay3 = 14;
const int master_resetPin = 34;

//uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x8E, 0xE7, 0x80};

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

struct_message cs1;
struct_message cs2;
struct_message cs3;

struct_message csStruct[3] = {cs1, cs2, cs3};
//--
/*
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
*/
//--

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  //char macStr[18];
  //Serial.print("Packet received from: ");
  //snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  csStruct[myData.id-1].d = myData.d;
  csStruct[myData.id-1].bs = myData.bs;
  csStruct[myData.id-1].ls = myData.ls;
  csStruct[myData.id-1].rst = myData.rst;
  Serial.printf("d value: %d \n", csStruct[myData.id-1].d);
  Serial.printf("bs value: %d \n", csStruct[myData.id-1].bs);
  Serial.printf("ls value: %d \n", csStruct[myData.id-1].ls);
  Serial.printf("rst value: %d \n", csStruct[myData.id-1].rst);
  Serial.println();
  /*
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
  Serial.println("Master reset State: ");
  Serial.println(myData.m_rst);
  Serial.println();
  */

  if (csStruct[myData.id-1].ls == false){
    if (myData.id == 1){
      //digitalWrite(ledPin, HIGH); //NEW ADD
      digitalWrite(relay, csStruct[myData.id-1].ls);
      Serial.println("IN 1 HIGH");
    }
    if (myData.id == 2){
      //digitalWrite(ledPin, HIGH); //NEW ADD
      digitalWrite(relay2, csStruct[myData.id-1].ls);
      Serial.println("IN 2 HIGH");
    }
    if (myData.id == 3){
      //digitalWrite(ledPin, HIGH); //NEW ADD
      digitalWrite(relay3, csStruct[myData.id-1].ls);
      Serial.println("IN 3 HIGH");
    }
    //digitalWrite(ledPin, HIGH);
    //digitalWrite(ledPin, myData.ls);
    //digitalWrite(ledPin, HIGH); //NEW ADD
    //digitalWrite(relay, csStruct[myData.id-1].ls);
    //digitalWrite(relay, HIGH);
  }
  
  if (csStruct[myData.id-1].rst == true){
      //digitalWrite(ledPin, LOW); //NEW ADD
      digitalWrite(relay, csStruct[myData.id-1].ls);
      digitalWrite(relay2, csStruct[myData.id-1].ls);
      digitalWrite(relay3, csStruct[myData.id-1].ls);
      //digitalWrite(relay, LOW);
  }

}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, LOW);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay3, HIGH);
  pinMode(master_resetPin, INPUT);
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
  myData.m_rst = digitalRead(master_resetPin);
  
  if (myData.m_rst == true){
    digitalWrite(relay, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    if (csStruct[myData.id-1].ls == false){
      csStruct[myData.id-1].ls = true;
      Serial.printf("Hello");
      Serial.println();
    }
  }
}
