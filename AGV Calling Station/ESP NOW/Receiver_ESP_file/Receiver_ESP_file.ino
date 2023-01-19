#include <esp_now.h>
#include <WiFi.h>

const int ledPin = 4;
const int relay = 12;
const int resetPin = 33;

// Structure example to receive data
// Must match the sender structure
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

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("Bool: ");
  Serial.println(myData.d);
  Serial.println();

  if (myData.d == true){
    //digitalWrite(ledPin, HIGH);
    //digitalWrite(ledPin, myData.ls);
    digitalWrite(ledPin, HIGH); //NEW ADD
    digitalWrite(relay, HIGH);
    //Serial.println("Current not Flowing");
    myData.lbs = myData.d; //added
    
  }
  
  if (resetPin == HIGH){
    digitalWrite(relay, LOW);
  }
  
  if (myData.rst == true && myData.ls == true){
    digitalWrite(ledPin, LOW); //NEW ADD
    digitalWrite(relay, LOW);
  }
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(relay, OUTPUT);
  pinMode(resetPin, INPUT);
  //digitalWrite(ledPin, myData.ls); //added
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}
