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

void setup() {
  Serial.begin(115200);
 
  dht.begin();

  u8g2.begin();
 
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenthinnerguys_tr);
  u8g2.drawStr(0,  9, "Hi! I'll check the");
  u8g2.drawStr(0, 20, "temperature and");
  u8g2.drawStr(0, 31, "humidity for you!");
  u8g2.sendBuffer();

  Serial.println("Thermometer starts!");
}

// use average of last x readings to smoothen sensory fluctuations
rolling_array<float_t, 5> arr_h;
rolling_array<float_t, 5> arr_c;

void loop() {
  delay(5000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float raw_h = dht.readHumidity();
  float raw_c = dht.readTemperature(); // pass true to get fahrenheit

  String l1("Temp: ");
  String l2("Temp 24h: ");
  String l3("Humidity: ");

  unsigned long tm = millis();
  if (isnan(raw_h) || isnan(raw_c)) {
    l1 = "ERR isnan h:" + String(isnan(raw_h));
    l1 += " c:" + String(isnan(raw_c));
  } else {
    arr_h.insert(raw_h);
    float h = arr_h.get_avg();

    arr_c.insert(raw_c);
    float c = arr_c.get_avg();

    float dev_c = 5;
    if (minmax.empty() || (c >= minmax.get_min() - dev_c && c <= minmax.get_max() + dev_c)) {
      minmax.add_val(tm, c);
    }

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
  u8g2.drawStr(0, 10, l1.c_str());
  u8g2.drawStr(0, 20, l2.c_str());
  u8g2.drawStr(0, 30, l3.c_str());
  u8g2.sendBuffer();

  Serial.println(l1);
  Serial.println(l2);
  Serial.println(l3);
}