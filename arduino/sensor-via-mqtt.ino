// secrets
#include "src/arduino_secrets.h"
const char* mqtt_user = MQTT_USER;
const char* mqtt_pw = MQTT_PW;

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

// MQTT
#include <PubSubClient.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson/wiki/API%20Reference

#define MQTT_VERSION MQTT_VERSION_3_1_1
const char* MQTT_CLIENT_ID = "bsmnt_computers";
const char* MQTT_SERVER_IP = "192.168.86.202";
const uint16_t MQTT_SERVER_PORT = 1883;

const char* MQTT_TOPIC_A = "bsmnt/temperature";
const char* MQTT_TOPIC_B = "bsmnt/humidity";


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const long utcOffsetInSeconds = -18000; //UTC -5.00 (-5 * 60 sec in min * 60 min in hr = -18000)

WiFiUDP wifi_udp;

NTPClient timeClient(wifi_udp, "pool.ntp.org", utcOffsetInSeconds);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

#define DHT11_PIN D5
#define DHTTYPE DHT11 
DHT dht = DHT(DHT11_PIN, DHTTYPE);

// function called to publish the temperature and the humidity
void publishData(float p_temperature, float p_humidity) {
  // create a JSON object
  StaticJsonDocument<200> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>(); // clear and replace with a new JsonObject
  root["temperature"] = (String)p_temperature;
  root["humidity"] = (String)p_humidity;
  serializeJsonPretty(root, Serial);
  Serial.println("");
  char data[200];
  serializeJson(root,data);
  mqttClient.publish(MQTT_TOPIC_A, data, true);
}

// function called when a MQTT message arrives
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(MQTT_CLIENT_ID, mqtt_user, mqtt_pw)) {
      Serial.println("INFO: connected");
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println("DEBUG: try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  
  WiFiManager wifiManager;  
  wifiManager.autoConnect();
  timeClient.begin();

  dht.begin();

  mqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  mqttClient.setCallback(callback);
}

void loop(){
  timeClient.update();

  if (!mqttClient.connected()) {
    reconnect();
  }

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

  if (isnan(h) || isnan(f)) {
    Serial.println("ERROR: Failed to read from DHT sensor!");
    return;
  } else {
    publishData(t, h);
  }

  delay(10000); //300000 = 5 mins
}
