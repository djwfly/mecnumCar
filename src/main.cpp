#include <Arduino.h>
#include "AFMotor.h"

#include <Servo.h> //引入舵机库文件
#include "Network.h"
#include "OSC.h"
#include "motorAction.h"

//#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
//int address = 0;

#define automouseMode 1
#define manualMode 0

//int differSpeed = 0;
//int defaultSpeed = 180;
int maxSpeed = 255;
int maxSteerSpeed = 100;
int runMode = automouseMode;

bool newControlParameters = false;
bool modifing_control_parameters = false;

float throttle;

int pwm[4] = {0, 0, 0, 0};

int moveActionID = 4;
int speedChanged = 0;
//espSerial(espRxPin, espTxPin);

void setup()
{
  /*   pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT); */

  pinMode(espRxPin, INPUT);
  pinMode(espTxPin, OUTPUT);

  // myservo.attach(10);//定义舵机接口（引脚9、10 都可以，且只能为这两个）
  Serial.begin(115200);
  espSerial.begin(115200);
  //differSpeed = EEPROM.read(address);
  Serial.print("load 'differSpeed' ifrom eeprom:");
  Serial.println(differSpeed);

  ///////------------
  OSC_init();
  delay(1000);
  espSerial.flush();
 /*  espSerial.print("+++"); // To ensure we exit the transparent transmision mode
  delay(100);

  ESPsendCommand("AT", "OK", 1);
  ESPsendCommand("AT+RST", "OK", 2); // ESP Wifi module RESET
  ESPwait("ready", 6);
  ESPsendCommand("AT+GMR", "OK", 5);

  // Deafault : we generate a wifi network
  espSerial.println("AT+CIPSTAMAC?");
  ESPgetMac();
  delay(200);
  ESPsendCommand("AT+CWQAP", "OK", 3);
  ESPsendCommand("AT+CWMODE=2", "OK", 3); // Soft AP mode
  // Generate Soft AP
  char *cmd = "AT+CWSAP=\"mechnumCar1_XX\",\"87654321\",5,3";
  // Update XX characters with MAC address (last 2 characters)
  //cmd[19] = MAC[10];
  // cmd[20] = MAC[11];
  ESPsendCommand(cmd, "OK", 6);
  // Start UDP SERVER on port 2222, telemetry port 2223
  Serial.println("Start UDP server");
  ESPsendCommand("AT+CIPMUX=0", "OK", 3);  // Single connection mode
  ESPsendCommand("AT+CIPMODE=1", "OK", 3); // Transparent mode
  ESPsendCommand("AT+CIPSTART=\"UDP\",\"192.168.4.2\",8000,9000,0", "OK", 3);
  delay(250);
  ESPsendCommand("AT+CIPSEND", ">", 2); // Start transmission (transparent mode) */
  delay(1000);
}

void loop() //主循环函数
{
  OSC_MsgRead();
  if (OSCnewMessage)
  {
    OSCnewMessage = 0;
    if (page == 1)
    {
      for (int i = 0; i < 9; i++)
      {
        if (push[i] == 1)
        {
          moveActionID = i;
          push[i] == 0;
          break;
        }
      }
      speedChanged = 1;
      throttle = fader[0];

      //OSC_MsgSend("/1/fader1\0\0\0,f\0\0\0\0\0\0", 20, fader[0]);
      //OSC_MsgSend("/1/fader2\0\0\0,f\0\0\0\0\0\0", 20, fader[1]);
    }
    else // set Page=2
    {
      throttle = 0;
      moveActionID = 4;
      speedChanged = 1;
      /*     for (int i = 0; i < 4; i++)
    {
      difPwm[i] = (int) ((fader[i + 1] - 0.5) * differSpeedDefault);
    }
    */
    }

    if (speedChanged == 1)
    {
      speedChanged = 0;
      int throttlePwmValue = map(int(throttle * 100), 0, 100, 0, maxSpeed);
      for (int i = 0; i < 4; i++)
      {
        pwm[i] = constrain((int)(throttlePwmValue + (fader[i + 1] - 0.5) * differSpeedDefault), 0, maxSpeed);
      }
    }

/*     Serial.println();
    Serial.print("fader0 : ");
    Serial.print(fader[0]);
    Serial.print("fader1 : ");
    Serial.print(fader[1]);
    Serial.print("fader2: ");
    Serial.print(fader[2]);
    Serial.print("push0 : ");
    Serial.print(push[0]);
    Serial.print("push1 : ");
    Serial.print(push[1]);
    Serial.print("toggle1 : ");
    Serial.print(toggle[0]);
    Serial.print("toggle2 : ");
    Serial.print(toggle[1]);
    Serial.println(); */
  }

  //  int throttlePwmValue = map(int(throttle * 100), 0, 100, 0, maxSpeed);

  // OSC_MsgSend("/1/label3\0\0\0,f\0\0\0\0\0\0", 20, _leftPwmVal);delay(15);
  // OSC_MsgSend("/1/label4\0\0\0,f\0\0\0\0\0\0", 20, _rightPwmVal);
      switch (moveActionID)
    {
    case 0:
      moveFL();
      break;
      ;
    case 1:
      moveF();
      break;
      ;
    case 2:
      moveFR();
      break;
      ;
    case 3:
      moveL();
      break;
      ;
    case 5:
      moveR();
      break;
      ;
    case 6:
      moveBL();
      break;
      ;
    case 7:
      moveB();
      break;
      ;
    case 8:
      moveBR();
      break;
      ;
    default:
      moveStop();
      break;
    }
   
}
