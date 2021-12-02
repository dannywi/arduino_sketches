#include <Arduino.h>
#include <short_float.h>
#include <rolling_array.h>
#include "DHT.h"
#include "U8g2lib.h"

// Rolling window
using float_t = short_float<2>;
rolling_minmax<float_t, unsigned long, 86400, 3600> minmax;

// DHT11 vars
const int DHTPIN = 2;
DHT dht(DHTPIN, DHT11, 6);

// OLED vars
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//#define PRINT_LOG

// use average of last x readings to smoothen sensory fluctuations
rolling_array<float_t, 4> arr_h;
rolling_array<float_t, 3> arr_c;

// states
float c = 0.0, c_min = 0.0, c_max = 0.0, h = 0;
unsigned long tm = 0;

//#define DISPLAY_V1
#define DISPLAY_V2

#ifdef DISPLAY_V1

void display_setup() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_tenthinnerguys_tr);
  u8g2.drawStr(0,  9, "Hi! I'll check the");
  u8g2.drawStr(0, 20, "temperature and");
  u8g2.drawStr(0, 31, "humidity for you!");
  u8g2.sendBuffer();
}

void display_loop() {
  String l1("Temp: ");
  String l2("Temp 24h: ");
  String l3("Humidity: ");

  if (c == 0.0 || h == 0.0) {
    l1 = "ERR isnan h:" + String(h == 0.0);
    l1 += " c:" + String(c == 0.0);
  } else {
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
}
#endif // DISPLAY_V1

#ifdef DISPLAY_V2

#define DISPLAY_WIDTH 128

size_t get_str_display_end_idx(const String& str, size_t st_idx) {
  size_t len = str.length();
  for (size_t i = st_idx; i < len; ++i) {
    size_t w = u8g2.getStrWidth(str.substring(st_idx, i).c_str());
    if (w >= DISPLAY_WIDTH)
      return i;
  }

  return len;
}

void scroll_display(const String& str, size_t x, size_t y) {
  size_t len = str.length();
  for (size_t i = 0; i < len; ++i) {
    u8g2.clearBuffer();
    size_t idx_to = get_str_display_end_idx(str, i);
    u8g2.drawStr(x, y, str.substring(i, idx_to).c_str());
    u8g2.sendBuffer();

    if (idx_to == len) {
      // last character appeared on screen
      delay(2000);
      break;
    } else {
      delay(i == 0 ? 2000 : 200);
    }
  }
}

void display_setup() {
  u8g2.setFont(u8g2_font_fub14_tf);
  scroll_display("Hi there! I'm a Thermometer!", 0, 23);
}

void display_loop() {
  static unsigned long prev = 0;
  const unsigned long interval = 30 * 1000L;

  if (tm - prev > interval) {
    prev = tm;
    u8g2.setFont(u8g2_font_fub14_tf);
    scroll_display(
      "Temperature 24h Min/Max: " + String(c_min, 1) + " - " + String(c_max, 1) + String("'C "),
      0, 23);
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_fub14_tf);
  u8g2.drawStr(0, 18, (String(c, 1) + String("'C")).c_str());
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(70, 10, (String("t:") + String(tm / (3600 * 1000.0), 2) + String("h")).c_str());
  u8g2.drawStr(70, 20, (String(c_min, 1) + String("-") + String(c_max, 1)).c_str());
  u8g2.drawStr(0, 30, (String("Humidity: ") + String(h, 1) + String(" %")).c_str());
  u8g2.sendBuffer();
}
#endif // DISPLAY_V2

void setup() {
  #ifdef PRINT_LOG
  Serial.begin(115200);
  #endif

  dht.begin();
  u8g2.begin();

  display_setup();

  #ifdef PRINT_LOG
  Serial.println("Thermometer starts!");
  #endif
}

void loop() {
  delay(5000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float raw_h = dht.readHumidity();
  float raw_c = dht.readTemperature(); // pass true to get fahrenheit

  tm = millis();
  if (isnan(raw_h) || isnan(raw_c)) {
    if (isnan(raw_h)) h = 0;
    if (isnan(raw_c)) c = 0;
  } else {
    arr_h.insert(raw_h);
    h = arr_h.get_avg();

    arr_c.insert(raw_c);
    c = arr_c.get_avg();

    float dev_c = 5;
    if (minmax.empty() || (c >= minmax.get_min() - dev_c && c <= minmax.get_max() + dev_c)) {
      minmax.add_val(tm, c);
    }

    c_min = minmax.get_min();
    c_max = minmax.get_max();
  }

  display_loop();
}