#include "ArduinoSTL.h"

int LATCH_PIN = 11;
int CLOCK_PIN = 9;
int DATA_PIN = 12;

byte leds = 0;
unsigned long lastActionTime = 0;

/*   The most common method of using 74CH595
 *   lctchPin->LOW : Begin transmitting signals.
 *   shiftOut(dataPin, clockPin, bitOrder, value)
 *   dataPin: the pin on which to output each bit. Allowed data types: int.
 *   clockPin: the pin to toggle once the dataPin has been set to the correct
 * value. Allowed data types: int. bitOrder: which order to shift out the bits;
 * either MSBFIRST or LSBFIRST. (Most Significant Bit First, or, Least
 * Significant Bit First). value: the data to shift out. Allowed data types:
 * byte. lctchPin->HIch : The end of the transmission signal.
 */
void updateShiftRegister() {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, leds);
  digitalWrite(LATCH_PIN, HIGH);
}

void flipBit(unsigned int b) {
  byte mask = 1 << b;
  leds = leds ^ mask;
}

void lightPattern(std::vector<int> pos, bool keep, int delayVal) {
  // first turn off all leds
  leds = 0;
  updateShiftRegister();
  delay(delayVal);

  // light up one by one
  for (int i : pos) {
    if (!keep)
      leds = 0;
    bitSet(leds, i);
    updateShiftRegister();
    delay(delayVal);
  }
}

void playPattern() {
  const int T_DELAY = 100;
  // bit 3 somehow matches Q1 in the diagram, and subsequent ones follow.
  lightPattern({ 3, 4, 5, 6, 5, 4, 3 }, false, T_DELAY);
  lightPattern({ 3, 4, 5, 6 }, true, T_DELAY);
  lightPattern({ 6, 5, 4, 3 }, true, T_DELAY);
  lightPattern({ 3, 5 }, true, T_DELAY);
  lightPattern({ 4, 6 }, true, T_DELAY);
  lightPattern({ 3, 4, 5, 6 }, true, T_DELAY);
  // turn off at the end
  leds = 0;
  updateShiftRegister();
}

void playPattern2() {
  size_t set = random(3, 10);
  for(size_t s = 0; s < set; ++s) {
    size_t pattern_size = random(5, 12);
    std::vector<int> pattern;
    for(size_t p = 0; p < pattern_size; ++p) {
      pattern.push_back(random(3, 8));
    }
    bool keep = static_cast<bool>(random(0, 2));
    int delayVal = random(80, 500);
    lightPattern(pattern, keep, delayVal);
  }
  // turn off at the end
  leds = 0;
  updateShiftRegister();
}

auto randWait = []() { return random(5000, 20000); };
unsigned long waitTime = 5000;

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  updateShiftRegister();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  waitTime = randWait();
  while (!Serial)
    ;  // Wait untilSerial is ready - Leonardo
  Serial.println("Enter LED Number 3 to 6 to toggle, 'x' for all, 'r' for pattern");
}
  
void loop() {
  if (millis() - lastActionTime > waitTime) {
    char buf[64];
    snprintf(buf, 64, "Waited %d ms, got bored, trying something ...", waitTime);
    Serial.println(buf);
    playPattern2();
    lastActionTime = millis();
    waitTime = randWait();
  }

  if (Serial.available())  // judge the serial if receive data
  {
    char ch = Serial.read();  // Read serial data and save it in "ch"

    if (ch >= '3' && ch <= '6')
    {
      int led = ch - '0';  // Convert char to int
      flipBit(led);
      //bitSet(leds, led);
      updateShiftRegister();
      char buf[32];
      snprintf(buf, 32, "Toggled LED [%d]\n", led);
      Serial.print(buf);
      delay(100);
      lastActionTime = millis();
    }

    if (ch == 'x') {
      byte all_ones = 255;
      leds = (leds == 0 ? all_ones : 0);
      updateShiftRegister();
      Serial.println("All Toggled");
      lastActionTime = millis();
    }

    if (ch == 'r') {
      Serial.println("Play Pattern ...");
      playPattern();
      lastActionTime = millis();
    }
  }
}
