#include <WiFiS3.h>
#include "secrets.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {}   // wait for the Serial Monitor

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(3000);
  }

  Serial.println("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength (dBm): ");
  Serial.println(WiFi.RSSI());
}

void loop() {}