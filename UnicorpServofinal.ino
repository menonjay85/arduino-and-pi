#include <Servo.h>
Servo myservo;

void setup() {
  
  myservo.attach(9); //attaches the servo on pin 9 
}

void loop() {
  int i=1;
  while(i<=4)
  {
  myservo.write(180); //rotate counterclockwise at slow speed
  delay(1000);
  myservo.write(90);
  delay(1000);
  i++;
  }
  myservo.write(0);
  delay(1000);
  myservo.write(90);
  delay(1000);
}
