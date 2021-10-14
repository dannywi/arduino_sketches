// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using HC-SR04 Module
// Tested on 17 September 2019
// ---------------------------------------------------------------- //

const int ECHO_PIN = 2;  // attach pin D2 Arduino to pin Echo of HC-SR04
const int TRIG_PIN = 3;  // attach pin D3 Arduino to pin Trig of HC-SR04

// RGB PIN
const int RED = 6;
const int GREEN = 10;
const int BLUE = 11;

const float SOUND_SPEED = 0.034;  // cm / microsec

const bool LOG = false;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);  // Sets the trigPin as an OUTPUT
  pinMode(ECHO_PIN, INPUT);   // Sets the ECHO_PIN as an INPUT

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  if (LOG) {
    Serial.begin(9600);  // Serial Communication starts at 9600 baudrate
    Serial.println("Ultrasonic Sensor HC-SR04 Test");
    Serial.println("with Arduino UNO R3");
  }
}

unsigned long getDistance() {
  // Clears the trigPin condition
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculating the distance (half time as it's round trip)
  unsigned long distance_cm = duration * SOUND_SPEED / 2;
  
  if (LOG) {
    // Displays the distance on the Serial Monitor
    char buf[32];
    snprintf(buf, 32, "Distance: %lu cm", distance_cm);
    Serial.println(buf);
  }

  return distance_cm;
}

void lightUp(unsigned long distance_cm) {
  const unsigned long DIST_MAX = 50;
  unsigned long capped_dist_cm = min(DIST_MAX, distance_cm);

  const byte RGB_MIN = 0;
  const byte RGB_MAX = 120;

  auto linear_inv = [](unsigned long v) {
    return RGB_MAX - v * (RGB_MAX - RGB_MIN) / static_cast<float>(DIST_MAX);
  };

  unsigned long val = linear_inv(capped_dist_cm);
  int pin = RED;
  if (val < RGB_MAX * 0.33)
    pin = BLUE;
  else if (val < RGB_MAX * 0.66)
    pin = GREEN;

  auto setPin = [&](int tgt) { analogWrite(tgt, pin == tgt ? val : 0); };
  setPin(RED);
  setPin(GREEN);
  setPin(BLUE);

  if (LOG) {
    char buf[32];
    snprintf(buf, 32, "LED val %lu", val);
    Serial.println(buf);
  }
}

void loop() {
  unsigned long distance_cm = getDistance();
  lightUp(distance_cm);
  delay(100);
}
