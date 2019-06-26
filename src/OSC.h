#ifndef OSC_H
#define OSC_H
#include <SoftwareSerial.h>
#define espRxPin  A4
#define espTxPin  A5
//#define OSCDEBUG
//#define OSCDEBUG2
//#define OSCDEBUG3

float    fader[5];
int      page = 1, push[9], toggle[2];
boolean  ChangePage;

char UDPBuffer[8]; // input message buffer

// OSC message internal variables
unsigned char OSCreadStatus;
unsigned char OSCreadCounter;
unsigned char OSCreadNumParams;
unsigned char OSCcommandType;
unsigned char OSCtouchMessage=0;
uint8_t OSCnewMessage;


SoftwareSerial espSerial(espRxPin, espTxPin);
// ------- OSC functions -----------------------------------------

// Aux functions
float OSC_extractParamFloat(uint8_t pos) {
  union {
    unsigned char Buff[4];
    float d;
  } u;

  u.Buff[0] = (unsigned char)UDPBuffer[pos];
  u.Buff[1] = (unsigned char)UDPBuffer[pos + 1];
  u.Buff[2] = (unsigned char)UDPBuffer[pos + 2];
  u.Buff[3] = (unsigned char)UDPBuffer[pos + 3];
  return (u.d);
}

int16_t OSC_extractParamInt(uint8_t pos) {
  union {
    unsigned char Buff[2];
    int16_t d;
  } u;

  u.Buff[1] = (unsigned char)UDPBuffer[pos];
  u.Buff[0] = (unsigned char)UDPBuffer[pos + 1];
  return (u.d);
}


void OSC_init()
{
  OSCreadStatus = 0;
  OSCreadCounter = 0;
  OSCreadNumParams = 0;
  OSCcommandType = 0;
  fader[0] = 0.5;
  fader[1] = 0.5;
  fader[2] = 0.5;
  fader[3] = 0.5;
  fader[4] = 0.5;
}

void OSC_MsgSend(char *c, unsigned char msgSize, float p)
{
  uint8_t i;
  union {
    unsigned char Buff[4];
    float d;
  } u;

  // We copy the param in the last 4 bytes
  u.d = p;
  c[msgSize - 4] = u.Buff[3];
  c[msgSize - 3] = u.Buff[2];
  c[msgSize - 2] = u.Buff[1];
  c[msgSize - 1] = u.Buff[0];
  for (i = 0; i < msgSize; i++)
  {
    espSerial.write((uint8_t)c[i]);
    //Serial.write((uint8_t)c[i]);
  }
}

void OSC_MsgRead()
{
  uint8_t i;
  uint8_t tmp;
  float value;
  float value2;

  // New bytes available to process?
  if (espSerial.available() > 0) {
    // We rotate the Buffer (we could implement a ring buffer in future)
    for (i = 7; i > 0; i--) {
      UDPBuffer[i] = UDPBuffer[i - 1];
    }
    UDPBuffer[0] = espSerial.read();delay(15);
    //Serial.print(UDPBuffer[0]);
#ifdef OSCDEBUG3
    Serial.print(UDPBuffer[0]);
#endif
    // We look for an OSC message start like /x/
     
    if ((UDPBuffer[0] == '/') && (UDPBuffer[2] == '/') && ((UDPBuffer[1] == '1') || (UDPBuffer[1] == '2')))
    {
      if (OSCreadStatus == 0)
      {
        page = UDPBuffer[1] - '0';  // Convert page to int
        OSCreadStatus = 1;
        OSCtouchMessage = 0;
        //Serial.print("$");
#ifdef OSCDEBUG3
        Serial.println();
#endif
      }
      else {
       // Serial.println("!ERR:osc");
        OSCreadStatus = 1;
      }
      return;
    }
    else if (OSCreadStatus == 1) { // looking for the message type
      // Fadder    /1/fader1 ,f  xxxx
      if ((UDPBuffer[3] == 'd') && (UDPBuffer[2] == 'e') && (UDPBuffer[1] == 'r')) {
        OSCreadStatus = 2;  // Message type detected
        OSCreadCounter = 11; // Bytes to read the parameter
        OSCreadNumParams = 1; // 1 parameters
        OSCcommandType = UDPBuffer[0] - '0';
#ifdef OSCDEBUG2
        Serial.print("$FAD1");
        Serial.print(OSCcommandType);
        Serial.print("$");
#endif
        return;
      } // end fadder
      // Push message
      if ((UDPBuffer[3] == 'u') && (UDPBuffer[2] == 's') && (UDPBuffer[1] == 'h')) {
        OSCreadStatus = 2;  // Message type detected
        OSCreadCounter = 10; // Bytes to read the parameter
        OSCreadNumParams = 1; // 1 parameters
        OSCcommandType = 20 + (UDPBuffer[0] - '0');
        //Serial.println(commandType);
#ifdef OSCDEBUG2
        Serial.print("$P");
          Serial.print(UDPBuffer[0] - '0');
          Serial.print(":");
#endif
          return;
        } // end push
        // Toggle message
        if ((UDPBuffer[3] == 'g') && (UDPBuffer[2] == 'l') && (UDPBuffer[1] == 'e')) {
        OSCreadStatus = 2;  // Message type detected
        OSCreadCounter = 10; // Bytes to read the parameter
        OSCreadNumParams = 1; // 1 parameters
        OSCcommandType = 30 + (UDPBuffer[0] - '0');
        //Serial.println(commandType);
#ifdef OSCDEBUG2
          Serial.print("$T");
          Serial.print(UDPBuffer[0] - '1');
          Serial.print(":");
#endif
          return;
        } // end toggle
      } 
      else if (OSCreadStatus == 2) 
      {
      if ((UDPBuffer[1] == '/') && (UDPBuffer[0] == 'z')) { // Touch up message? (/z) [only on page1]
        if ((page == 1) && (OSCcommandType <= 2)) { // Touchup message only on Fadder1 and Fadder2
          OSCtouchMessage = 1;
        }
        else {
          OSCtouchMessage = 0;
          OSCreadStatus = 0; //Finish
        }
      } // Touch message(/z)
      OSCreadCounter--;   // Reading counter until we reach the Parameter position
      if (OSCreadCounter <= 0) {
        espSerial.flush();
        OSCreadStatus = 0;
        OSCnewMessage = 1;
        //Serial.println(value);
        switch (OSCcommandType) {
          case 0:
            value = OSC_extractParamFloat(0);
            //Serial.print("fader1 : ");Serial.println(value);
            fader[0] = value;
/*             if ((OSCtouchMessage) && (value == 0)) {
              fader[0] = 0.5;
              //Serial.println("TOUCH_X");
              OSC_MsgSend("/1/fader0\0\0\0,f\0\0\0\0\0\0", 20, 0.5); 
            }*/
#ifdef OSCDEBUG
            Serial.print("$F1:");
            Serial.println(fader[0]);
#endif
            break;
          case 1:
            value = OSC_extractParamFloat(0);
            //Serial.println("fader2 : ");Serial.print(value);
            fader[1] = value;
            /* if ((OSCtouchMessage) && (value == 0)) {
              fader[1] = 0.5;
              //Serial.println("TOUCH_Y");
              OSC_MsgSend("/2/fader1\0\0\0,f\0\0\0\0\0\0", 20, 0.5); 
            }*/
#ifdef OSCDEBUG
            Serial.print("$F2:");
            Serial.println(fader[1]);
#endif
            break;
          case 2:
            fader[2] = OSC_extractParamFloat(0);
#ifdef OSCDEBUG
            Serial.print("$F3:");
            Serial.println(fader[2]);
#endif
            break;
          case 3:
            fader[3] = OSC_extractParamFloat(0);
#ifdef OSCDEBUG
            Serial.print("$F4:");
            Serial.println(fader[3]);
#endif
            break;
           
          case 4:
            fader[4] = OSC_extractParamFloat(0);
#ifdef OSCDEBUG
            Serial.print("$F4:");
            Serial.println(fader[4]);
#endif
            break;

            default:
            // Push y toggle
            value = OSC_extractParamFloat(0);
            if ((OSCcommandType >= 20) && (OSCcommandType < 29))
            {
              if (value == 0)
                push[OSCcommandType - 20] = 0;
              else
                push[OSCcommandType - 20] = 1;
            }
            if ((OSCcommandType >= 30) && (OSCcommandType < 35))
            {
              if (value == 0)
                toggle[OSCcommandType - 30] = 0;
              else
                toggle[OSCcommandType - 30] = 1;
            }
            break;
        } // switch
      }  // if (OSCRead_counter<=0)
    }  // if (OSCread_status==2)
  }  // end Serial.available()
}

  #endif
