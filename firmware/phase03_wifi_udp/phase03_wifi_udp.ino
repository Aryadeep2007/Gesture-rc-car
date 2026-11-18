#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "secrets.h"

const unsigned int LOCAL_PORT = 4210;
WiFiUDP udp;
char packet[64];

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(3000);
  }
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  udp.begin(LOCAL_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(LOCAL_PORT);
}

void loop() {
  int size = udp.parsePacket();
  if (size > 0) {
    int len = udp.read(packet, sizeof(packet) - 1);
    packet[len] = '\0';
    Serial.print("Received: ");
    Serial.println(packet);

    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.print("ACK");
    udp.endPacket();
  }
}