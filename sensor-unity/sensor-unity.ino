#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Lenovo K8 Plus 1090";
const char* password = "87654321";

IPAddress ip(192,168,43,135);
IPAddress gateway(192,168,43,1);
IPAddress subnet(255,255,255,0); 

WiFiServer server(80);
WiFiClient client;

int SensorState;

#define echoPin D5

void setup() {
  Serial.begin(115200);

  pinMode(echoPin, INPUT);

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
    }

  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
  

}

void loop() {
  client = server.available();

  if(client)
  {
    Serial.println("Client Connected");

    while(client.connected())
    {
      SensorState = digitalRead(echoPin);

      if(SensorState == 0)
      {
        Serial.println(SensorState);

        client.print(SensorState);
        client.print('\r');
        }
       else
       {
        Serial.println(SensorState);

        client.print(SensorState);
        client.print('\r');
        }

        delay(500);
      }
    }

}
