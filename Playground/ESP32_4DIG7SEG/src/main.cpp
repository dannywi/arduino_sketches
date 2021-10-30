#include <Arduino.h>

#include "XDIG7SEG.h"

// https://kit.socinno.com/1_3_b/

int pinA = 16;
int pinB = 17;
int pinC = 18;
int pinD = 19;
int pinE = 21;
int pinF = 22;
int pinG = 23;
int pinDot = 4;

int D1 = 14;
int D2 = 27;
int D3 = 26;
int D4 = 25;

XDIG7SEG setter{{pinA, pinB, pinC, pinD, pinE, pinF, pinG},
                      {D1, D2, D3, D4}, pinDot};

void setup() {
  //Serial.begin(9600);
  setter.setup();
}

void loop() {
  setter.set(setter.convert_to_digit("saturday"));
  setter.set(setter.convert_to_digit("2021.10.30"));
  setter.set(setter.convert_to_digit("20.5c"), 3000);
  setter.set(setter.convert_to_digit("34.8h"), 3000);
  /*
  TODO:
  - add features, e.g. nice blinking patterns, moving lines, etc.
  */
}
