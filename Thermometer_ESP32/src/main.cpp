#include <Arduino.h>
#include "DHT.h"
#include "XDIG7SEG.h"

// DHT11 pins
const int DHTPIN = 32;
const int DHTTYPE = DHT11;

// 5641AS pins
const int pinA = 16;
const int pinB = 17;
const int pinC = 18;
const int pinD = 19;
const int pinE = 21;
const int pinF = 22;
const int pinG = 23;
const int pinDot = 4;

const int D1 = 14;
const int D2 = 27;
const int D3 = 26;
const int D4 = 25;

DHT dht(DHTPIN, DHTTYPE);

XDIG7SEG setter{{pinA, pinB, pinC, pinD, pinE, pinF, pinG},
                      {D1, D2, D3, D4}, pinDot};

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  setter.setup();
}

void loop() {
// Wait a few seconds between measurements.
  delay(4000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Read temperature as Fahrenheit (isFahrenheit = true)
  // float f = dht.readTemperature(true);

  //Serial.printf("Humidity: %f - Temperature: %f Cel\n", h, t);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  char buf[32];
  snprintf(buf, sizeof(buf), "humidity=%.1fpct", h);
  setter.set(setter.convert_to_digit(buf));
  Serial.println(buf);

  snprintf(buf, sizeof(buf), "temperature=%.1fcel", t);
  setter.set(setter.convert_to_digit(buf));
  Serial.println(buf);

  snprintf(buf, sizeof(buf), "heat idx=%.1fcel", hic);
  setter.set(setter.convert_to_digit(buf));
  Serial.println(buf);

  //Serial.printf("Humidity: %.1f - Temperature: %.1f Cel - Heat Index: %.1f Cel\n", h, t, hic);
}