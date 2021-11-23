// secrets
#include "src/arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Serial Peripheral
#include <SPI.h>

// Wifi
#include <WiFiManager.h>
#include <ESP8266WiFi.h>

// Time
#include <NTPClient.h>

// Sensor Specific Libraries (using a dht 11)
#include <Adafruit_Sensor.h>
#include <DHT.h>

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const long utcOffsetInSeconds = -18000; //UTC -5.00 (-5 * 60 sec in min * 60 min in hr = -18000)

WiFiUDP wifi_udp;

NTPClient timeClient(wifi_udp, "pool.ntp.org", utcOffsetInSeconds);

#define DHT11_PIN D5
#define DHTTYPE DHT11 
DHT dht = DHT(DHT11_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  WiFiManager wifiManager;  
  wifiManager.autoConnect();
  timeClient.begin();

  dht.begin();
}

void loop(){
  timeClient.update();

  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();

  bool isPM = false;
  if(hh > 12) {
    hh=hh-12;
    isPM = true;
    }

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(hh);
  Serial.print(":");
  Serial.print(mm);
  Serial.print(".");
  Serial.println(ss);

  //int chk = DHT.read11(DHT11_PIN);
  float h = dht.readHumidity();
  float t = dht.readTemperature(); //Celsius
  float f = dht.readTemperature(true); //Fahrenheit

  Serial.print(t);
  Serial.print("\xC2\xB0");
  Serial.print("C | ");
  Serial.print(f);
  Serial.print("\xC2\xB0");
  Serial.println("F");
  Serial.print(h);
  Serial.println("% Humidity");
  Serial.println();

  delay(5000);
}
