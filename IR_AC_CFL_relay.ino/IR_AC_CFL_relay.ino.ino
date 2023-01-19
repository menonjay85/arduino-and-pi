                       
int inputPin = 4; // choose input pin (for Infrared sensor) 
int val = 0;
#define RELAY1  7                        
void setup()

{    


Serial.begin(9600);
  pinMode(RELAY1, OUTPUT);  
  pinMode(inputPin, INPUT);     

}

void loop(){
val = digitalRead(inputPin); // read input value 
   if (val == HIGH)
   { 
   digitalWrite(RELAY1,0);           // Turns ON Relays 1
   Serial.println("Light ON");
   delay(100);   
   } 
   else 
   { 
   digitalWrite(RELAY1,1);           // Turns ON Relays 1
   Serial.println("Light ON");
   delay(1000);
   } 
}
