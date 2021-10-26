#include <Arduino.h>
#include <WiFi.h>
#include "time.h"

#define LED_FREQ        5000
#define LED_CHANNEL     0
#define LED_RESOLUTION  8
#define LED             22

#define DHTTYPE DHT11
#define DHTPIN 19

void setup() {
  Serial.begin(9600);
  ledcSetup(LED_CHANNEL, LED_FREQ, LED_RESOLUTION);
  ledcAttachPin(LED, LED_CHANNEL);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(3000);

  Serial.println("Setup done");
}

const char* ssid = "";
const char* password = "";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("");
  Serial.print("Connected, my IP: ");
  Serial.println(WiFi.localIP());
}

void scanWiFi() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}


void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    configTime(0, 0, "pool.ntp.org");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S GMT");
}

uint8_t cnt = 0;

void loop() {
  ledcWrite(LED_CHANNEL, 50);
  delay(1500);
  ledcWrite(LED_CHANNEL, 0);
  delay(1500);

  if (WiFi.status() != WL_CONNECTED) {
    scanWiFi();
    initWiFi();
  }

  Serial.print("Counting ... ");
  Serial.println(cnt);
  cnt = cnt == 255 ? 0 : cnt + 1;

  printLocalTime();
}