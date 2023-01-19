#include <DS3231.h>
#include "Wire.h"

int IN1 = 4;

DS3231 rtc;


const int OnHour = 15;
const int OnMin = 10;
const int OffHour = 15;
const int OffMin = 15;

void setup() {
  Serial.begin(115200);
  
  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);
}

void loop() {
  char t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hour(s), ");
  Serial.print(t.min);
  Serial.print(" minute(s)");
  Serial.println(" ");
  delay (1000);
  
  if(t.hour == OnHour && t.min == OnMin && t.min <OffMin){
    digitalWrite(IN1,HIGH);
    Serial.println("LIGHT ON");
    }
    
    else if(t.hour > OffHour && t.hour == OffHour && t.min == OffMin){
      digitalWrite(IN1,LOW);
      Serial.println("LIGHT OFF");
    }
}
