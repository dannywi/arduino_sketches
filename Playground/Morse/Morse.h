#ifndef MORSE_H
#define MORSE_H



class Morse {
 public:
  Morse(int pin) : _pin(pin) {}

  void dot() const {
    digitalWrite(_pin, HIGH);
    delay(250);
    digitalWrite(_pin, LOW);
    delay(250);
  }

  void dash() const {
    digitalWrite(_pin, HIGH);
    delay(1000);
    digitalWrite(_pin, LOW);
    delay(250);
  }

 private:
  int _pin;
};

#endif