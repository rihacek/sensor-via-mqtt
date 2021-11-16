// secrets
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Serial Peripheral
#include <SPI.h>

// Wifi
#include <WiFiManager.h>
#include <ESP8266WiFi.h>

// Time
#include <NTPClient.h>

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const long utcOffsetInSeconds = -18000; //UTC -5.00 (-5 * 60 sec in min * 60 min in hr = -18000)

WiFiUDP wifi_udp;

NTPClient timeClient(wifi_udp, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  WiFiManager wifiManager;  
  
  //use this to configure the chip's wifi:
  //wifiManager.autoConnect(ssid, pass);

  // use this after the chip's wifi is configured (remove ssid, pass variables as well)
  wifiManager.autoConnect();

  /*  autoConnect() is interesting in that it will create 
   *  an Access Point with the SSID and Password provided.
   *  You then connect to that AP and "sign in" to configure
   *  the Wifi memory for the actual AP that you want to use.
   *  After configuring, the chip will connect to that AP rather 
   *  than creating a temporary access point.
   */

  timeClient.begin();
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
  Serial.print(":");
  Serial.println(ss);
  
  delay(5000);
}
