/*****************************************************
 * Date: 13 july 2018
 * Written by: Usman Ali Butt
 * Property off: www.microcontroller-project.com
 * ***************************************************/
 
#include <ESP8266WiFi.h>
#include <Servo.h>
Servo servo;
 
const char* ssid = "iPhone";
const char* password = "jenika3mehta";  

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
  servo.attach(2); //Gpio-2 of nodemcu with pwm pin of servo motor
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
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
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
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 int value = 0;
 // Match the request

if (request.indexOf("/Req=90") != -1)  {
  servo.write(0); //Moving servo to 0 degree
  servo.detach();
}
if (request.indexOf("/Req=0") != -1)  {  
  servo.write(90); //Moving servo to 90 degree
  servo.detach();
}

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center>Servo motor control over WiFi</h1><br><br>");
  client.print("Current position  = ");
  client.print(value); 
  client.println("<br><br>");
  client.println("<a href=\"/Req=90\"\"><button>ON</button></a>");
  client.println("<a href=\"/Req=0\"\"><button>OFF</button></a>");

  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
