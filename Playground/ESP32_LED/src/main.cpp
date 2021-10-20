#include <Arduino.h>

#define LED 22

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

uint8_t counter = 0;
void loop() {
  Serial.print("HELLO WORLD!!! - ");
  Serial.println(counter++);
  if (counter == 255)
    counter = 0;

  digitalWrite(LED, HIGH);
  delay(1500);
  digitalWrite(LED, LOW);
  delay(1500);
}