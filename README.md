##sensor-via-mqtt

Small POC to get sensor data over to an MQTT broker

- sensor doesn't really matter; starting with light sensor for ease
- wifi enabled
- time pulled from NTP

###Note
I've included some info on configuring the ESP8266 WiFi chip in the comments. You can reset what's stored on the chip by changing board settings.

###Libraries:
- WifiManager - https://github.com/tzapu/WiFiManager
- NTPClient - https://github.com/arduino-libraries/NTPClient
