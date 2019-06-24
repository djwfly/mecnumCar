#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
WiFiUDP UdP;
const char *ssid = "autoCar";
const char *password = "87654321";
uint16_t   RxPort = 9000;
uint16_t   TxPort = 8000;
uint8_t PacketBuffer[128];

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  UdP.begin(RxPort);
  ESP.wdtEnable(5000);
}

void loop()
{
  ESP.wdtFeed();
  int PacketSize = UdP.parsePacket();
  if (PacketSize) 
  {UdP.read(PacketBuffer,PacketSize); 
     Serial.write(PacketBuffer,PacketSize);
     Serial.println();}
delay(50);
  while (Serial.available()>0)
  {
    char outChar = Serial.read();
    UdP.beginPacket(UdP.remoteIP(), TxPort);
    UdP.write(outChar);
    UdP.endPacket();
    ESP.wdtFeed();
  }
  delay(50);
}
