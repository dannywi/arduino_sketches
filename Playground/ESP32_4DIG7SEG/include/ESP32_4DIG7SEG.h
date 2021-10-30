#ifndef ESP32_4DIG7SEG_H
#define ESP32_4DIG7SEG_H

#include <Arduino.h>

#include <array>
#include <map>
#include <vector>

// Tested on 5641AS vs ESP32 DevKitC

const size_t SEG_SIZE = 7;
const size_t DIG_SIZE = 4;

class ESP32_4DIG7SEG {
 public:
  /*
    - 5641AS connection pins - (S)egment (D)igit:
        upper:  D1 S1 S6   D2 D3 S2
                 .  .  .    .  .  .
                 .  .  .    .  .  .
        lower:  S5 S4      S3 S7 D4
    - digits: 1 - 4 left to right
  */
  ESP32_4DIG7SEG(const std::array<int, SEG_SIZE>& segment_pins,
                 const std::array<int, DIG_SIZE>& digit_pins)
      : _segment_pins(segment_pins), _digit_pins(digit_pins) {}

  void setup() {
    for (auto pin : _segment_pins) {
      pinMode(pin, OUTPUT);
    }
    for (auto pin : _digit_pins) {
      pinMode(pin, OUTPUT);
    }
  }

  // terop the display if more than 4 characters
  bool set(std::vector<char> digits) const {
    if (digits.size() != DIG_SIZE) {
      std::vector<char> suffix(
          digits.size() < DIG_SIZE ? DIG_SIZE - digits.size() : 3, ' ');
      digits.insert(digits.end(), suffix.begin(), suffix.end());
    }

    // terop
    size_t end_i = DIG_SIZE;
    while (end_i <= digits.size()) {
      std::array<char, DIG_SIZE> sub_array;
      auto start_it = digits.begin() + end_i - DIG_SIZE;
      for (size_t i = 0; i < DIG_SIZE; ++i, ++start_it)
        sub_array[i] = *start_it;

      ++end_i;
      const int DELAY = 800;
      unsigned long start_time = millis();
      while (millis() - start_time < DELAY) set_fixed_len(sub_array);
    }

    return true;
  }

  bool set(const std::string& str) {
    Serial.println(str.c_str());
    std::vector<char> v{str.begin(), str.end()};
    return set(v);
  }

 private:
  bool set_fixed_len(const std::array<char, DIG_SIZE>& digits) const {
    const static int DELAY = 5;
    for (int i = 0; i < DIG_SIZE; ++i) {
      set(i, digits[i]);
      delay(DELAY);
    }
    return true;
  }

  bool set(int digit, char c) const {
    if (digit > 3) return false;
    auto it = dict.find(c);
    if (it == dict.end()) return false;

    for (auto pin : _digit_pins) digitalWrite(pin, HIGH);
    digitalWrite(_digit_pins[digit], LOW);

    byte segments = it->second;
    for (int i = 0; i < SEG_SIZE; ++i)
      digitalWrite(_segment_pins[i], (segments & (1 << (7 - i))) ? HIGH : LOW);
    return true;
  }

  const static byte A = 1 << 7;
  const static byte B = 1 << 6;
  const static byte C = 1 << 5;
  const static byte D = 1 << 4;
  const static byte E = 1 << 3;
  const static byte F = 1 << 2;
  const static byte G = 1 << 1;

  const std::map<char, byte> dict{
      {'0', A | B | C | D | E | F},
      {'1', B | C},
      {'2', A | B | D | E | G},
      {'3', A | B | C | D | G},
      {'4', B | C | F | G},
      {'5', A | C | D | F | G},
      {'6', A | C | D | E | F | G},
      {'7', A | B | C},
      {'8', A | B | C | D | E | F | G},
      {'9', A | B | C | D | F | G},
      {'-', G},
      {'_', D},
      {' ', 0},
      {'a', A | B | C | D | E | G},
      {'b', C | D | E | F | G},
      {'c', D | E | G},
      {'d', B | C | D | E | G},
      {'e', A | B | D | E | F | G},
      {'f', A | E | F | G},
      {'g', A | B | C | D | F | G},
      {'h', C | E | F | G},
      {'i', C},
      {'j', B | C | D},
      {'k', A | C | E | F | G},
      {'l', D | E | F},
      {'m', A | C | E},
      {'n', C | E | G},
      {'o', C | D | E | G},
      {'p', A | B | E | F | G},
      {'q', A | B | C | F | G},
      {'r', E | G},
      {'s', A | C | D | F | G},
      {'t', D | E | F | G},
      {'u', C | D | E},
      {'v', C | D | E},
      {'w', B | D | F},
      {'x', B | C | E | F | G},
      {'y', B | C | D | F | G},
      {'z', A | B | D | E | G},
  };
  /*
      A
     ---
  F |   | B
    | G |
     ---
  E |   | C
    | D |
     ---
  */

  // in order segment A - G
  std::array<int, SEG_SIZE> _segment_pins;

  // in order D1 to D4
  std::array<int, DIG_SIZE> _digit_pins;
};

#endif