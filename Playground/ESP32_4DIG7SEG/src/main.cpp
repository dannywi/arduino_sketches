#include <Arduino.h>

#include "ESP32_4DIG7SEG.h"

// https://kit.socinno.com/1_3_b/

int pinA = 16;
int pinB = 17;
int pinC = 18;
int pinD = 19;
int pinE = 21;
int pinF = 22;
int pinG = 23;

int D1 = 14;
int D2 = 27;
int D3 = 26;
int D4 = 25;

ESP32_4DIG7SEG setter{{pinA, pinB, pinC, pinD, pinE, pinF, pinG},
                      {D1, D2, D3, D4}};

void setup() {
  //Serial.begin(9600);
  setter.setup();
}

void loop() {
  setter.set("saturday");
  setter.set("2021-10-29");

  /*
  TODO:
  - need ability to light up the dots
  - add features, e.g. nice blinking patterns, moving lines, etc.
  */
}
