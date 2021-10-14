#include "Arduino.h"
#include "Morse.h"

const int LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  static Morse m{LED_PIN};
  
  m.dot(); m.dot(); m.dot();
  m.dash(); m.dash(); m.dash();
  m.dot(); m.dot(); m.dot();

  delay(3000);
}
