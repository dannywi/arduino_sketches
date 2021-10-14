const int BUZZER = 12;  // the pin of the active buzzer

void setup() {
  pinMode(BUZZER, OUTPUT);  // initialize the buzzer pin as an output
}

void makeSound(unsigned char delayVal) {
  const size_t duration = 500;  // ms
  const size_t stopCnt = 0.5 * duration / delayVal;
  for (size_t i = 0; i < stopCnt; ++i) {
    digitalWrite(BUZZER, HIGH);
    delay(delayVal);  // wait for 1ms
    digitalWrite(BUZZER, LOW);
    delay(delayVal);  // wait for 1ms
  }
}

void loop() {
  makeSound(8);
  makeSound(2);
  makeSound(8);
  makeSound(1);

  delay(2000);
}
