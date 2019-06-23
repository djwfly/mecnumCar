#ifndef NETWORK_H
#define NETWORK_H

#include "osc.h"
#define TELEMETRY "192.168.4.1"
String MAC;  // MAC address of Wifi module


int ESPwait(String stopstr, int timeout_secs)
{
  String response;
  bool found = false;
  char c;
  long timer_init;
  long timer;

  timer_init = millis();
  while (!found) {
    timer = millis();
    if (((timer - timer_init) / 1000) > timeout_secs) { // Timeout?
      Serial.println("!Timeout!");
      return 0;  // timeout
    }
    if (espSerial.available()) {
      c = espSerial.read();
      Serial.print(c);
      response += c;
      if (response.endsWith(stopstr)) {
        found = true;
        delay(10);
        espSerial.flush();
        Serial.println();
      }
    } // end espSerial_available()
  } // end while (!found)
  return 1;
}

// getMacAddress from ESP wifi module
int ESPgetMac()
{
  char c1, c2;
  bool timeout = false;
  long timer_init;
  long timer;
  uint8_t state = 0;
  uint8_t index = 0;

  MAC = "";
  timer_init = millis();
  while (!timeout) {
    timer = millis();
    if (((timer - timer_init) / 1000) > 5) // Timeout?
      timeout = true;
    if (espSerial.available()) {
      c2 = c1;
      c1 = espSerial.read();
      Serial.print(c1);
      switch (state) {
        case 0:
          if (c1 == ':')
            state = 1;
          break;
        case 1:
          if (c1 == '\r') {
            MAC.toUpperCase();
            state = 2;
          }
          else {
            if ((c1 != '"') && (c1 != ':'))
              MAC += c1;  // Uppercase
          }
          break;
        case 2:
          if ((c2 == 'O') && (c1 == 'K')) {
            Serial.println();
            espSerial.flush();
            return 1;  // Ok
          }
          break;
      } // end switch
    } // Serial_available
  } // while (!timeout)
  Serial.println("!Timeout!");
  espSerial.flush();
  return -1;  // timeout
}

int ESPsendCommand(char *command, String stopstr, int timeout_secs)
{
  espSerial.println(command);
  ESPwait(stopstr, timeout_secs);
  delay(250);
}
#endif
