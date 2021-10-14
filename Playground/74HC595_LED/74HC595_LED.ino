// www.elegoo.com
// 2016.12.9

#include "ArduinoSTL.h"

const int LATCH_PIN = 11;  // (11) ST_CP [RCK] on 74HC595
const int CLOCK_PIN = 9;   // (9) SH_CP [SCK] on 74HC595
const int DATA_PIN = 12;   // (12) DS [S1] on 74HC595

/*  The most common method of using 74CH595
    LATCH_PIN->LOW :
      Begin transmitting signals.
    shiftOut(DATA_PIN, CLOCK_PIN, bitOrder, value)
      DATA_PIN:
        the pin on which to output each bit. Allowed data types: int.
      CLOCK_PIN:
        the pin to toggle once the dataPin has been set to the correct
        value. Allowed data types: int.
      bitOrder:
        which order to shift out the bits; either MSBFIRST or LSBFIRST.
        (Most Significant Bit First, or, Least Significant Bit First).
      value: the data to shift out. Allowed data types: byte.
    LATCH_PIN->HIGH : The end of the transmission signal.
*/
void updateShiftRegister(byte leds) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, leds);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void light_pattern(std::vector<int> pos, bool keep, int delayVal) {
  // first turn off all leds
  byte leds = 0;
  updateShiftRegister(leds);
  delay(delayVal);

  // light up one by one
  for (int i : pos) {
    if (!keep)
      leds = 0;
    bitSet(leds, i);
    updateShiftRegister(leds);
    delay(delayVal);
  }
}

void loop() {
  const int T_DELAY = 500;
  // bit 3 somehow matches Q1 in the diagram, and subsequent ones follow.
  light_pattern({ 3, 4, 5, 6, 5, 4, 3 }, false, T_DELAY);
  light_pattern({ 3, 4, 5, 6 }, true, T_DELAY);
  light_pattern({ 6, 5, 4, 3 }, true, T_DELAY);
  light_pattern({ 3, 5 }, true, T_DELAY);
  light_pattern({ 4, 6 }, true, T_DELAY);
}
