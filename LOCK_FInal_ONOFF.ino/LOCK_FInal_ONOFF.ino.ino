#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo ser;
const char* ssid = "iPhone";
const char* password = "jenika3mehta";

int pos=0;
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(12,OUTPUT);
  
  ser.attach(14); //GPIO14 is D5
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  String m;
  if (req.indexOf("/D1/0") != -1){
    m = "D1 is Low";
    ser.attach(14);
    ser.write(180);// waits 15ms for the servo to reach the position
    delay(1000);
    ser.detach();
    ser.write(0);
    delay(2000);
  }
  
  else if (req.indexOf("/D1/1") != -1){
    m = "D1 is High";
    ser.attach(14);
    ser.write(0);
    delay(1000); 
    ser.detach();     
  }

  else if (req.indexOf("/D2/0") != -1){
    m = "D2 is Low";
    digitalWrite(12, 0);
  }
  else if (req.indexOf("/D2/1") != -1){
    m = "D2 is High";
    digitalWrite(12, 1);
  }
  
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<head><meta http-equiv=\"Access-Control-Allow-Origin\" content=\"*\"></head>\n<html>\r\n" +m;
  s += "</html>\n";
  
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
