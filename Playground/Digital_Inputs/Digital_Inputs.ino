#include "ArduinoSTL.h"

const int LED_PIN = 5;
const int BUTTON_A_PIN = 9;
const int BUTTON_B_PIN = 8;

const bool LOG = true;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  if (LOG) Serial.begin(9600);
}

const std::vector<int> levels = { 20, 100, 200 };
size_t currIdx = 0;

size_t getIndex() {
  size_t ret = currIdx;
  currIdx = currIdx == levels.size() - 1 ? 0 : currIdx + 1;

  if (LOG) {
    char buf[64];
    snprintf(buf, 64, "ret [%d] currIdx [%d] level [%d]", ret, currIdx, levels[ret]);
    Serial.println(buf);
  }

  return ret;
}

void loop()
{
  if (digitalRead(BUTTON_A_PIN) == LOW)
  {
    analogWrite(LED_PIN, levels[getIndex()]);
    delay(500);
  }
  if (digitalRead(BUTTON_B_PIN) == LOW)
  {
    digitalWrite(LED_PIN, LOW);
    currIdx = 0;
  }
}
