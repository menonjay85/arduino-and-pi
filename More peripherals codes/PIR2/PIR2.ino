int pirPin = 13;
int ledPin = 12;

int state = LOW;
 
void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
}
 
void loop(){
  if (digitalRead(pirPin) == HIGH) {
              
    digitalWrite(ledPin, HIGH);
    if (state == LOW) {
      Serial.println("Motion detected");
      state = HIGH;
    }
    
  } else {
    
    digitalWrite(ledPin, LOW);
    if (state == HIGH){
      state = LOW;
    }
    
  }
}
