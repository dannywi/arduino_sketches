#include <Arduino.h>
#include <short_float.h>
#include <rolling_array.h>
#include "DHT.h"
#include "U8g2lib.h"

// Rolling window
using float_t = short_float<2>;
rolling_minmax<float_t> minmax;

// DHT11 vars
const int DHTPIN = 2;
DHT dht(DHTPIN, DHT11, 6);

// OLED vars
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
const int row1 = 10;
const int row2 = 21;
const int row3 = 32;

void setup() {
  Serial.begin(9600);
 
  dht.begin();

  u8g2.begin();
 
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(0, row1, "Let there be warmth!");
  u8g2.drawStr(0, row3, "copyright: Danny W");
  u8g2.sendBuffer();
 
  Serial.println("Thermometer!");
}

void loop() {
  delay(5000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float c = dht.readTemperature(); // pass true to get fahrenheit

  String l1("Temp: ");
  String l2("Temp 24h: ");
  String l3("Humidity: ");

  unsigned long tm = millis();
  if (isnan(h) || isnan(c)) {
    l1 = "ERR isnan h:" + String(isnan(h));
    l1 += " c:" + String(isnan(c));
  } else {
    minmax.add_val(tm, c);
    float c_min = minmax.get_min();
    float c_max = minmax.get_max();
  
    l1 += String(c, 1) + String("'C");;
    l2 += String(c_min, 1) + String("-") + String(c_max, 1) + String("'C");;
    l3 += String(h, 1) + String(" %");
  }

  // print time in hours
  l1 += String(" t:") + String(tm / (3600000.0),2) + String("h");
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(0, row1, l1.c_str());
  u8g2.drawStr(0, row2, l2.c_str());
  u8g2.drawStr(0, row3, l3.c_str());
  u8g2.sendBuffer();

  Serial.println(l1);
  Serial.println(l2);
  Serial.println(l3);
}

/*
TODO:
- try commenting out u8g2, see how much memory it gains
- try not printing serialize, see how much memorty it gains
- make tests for the classes
- add pressure sensor
- add time since start (in days?)
- reset in ulong's limit
*/