#include <Arduino.h>
#include <LibRobus.h>

void setup() {
  
  BoardInit();
  pinMode(37, INPUT);
  while(digitalRead(37)==0)
  {
    delay(10);
    Serial.println(digitalRead(37));
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("starterrrr");
  delay(1000);
}