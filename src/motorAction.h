#ifndef MOTORACTION_H
#define MOTORACTION_H

#include <AFMotor.h>

extern int pwm[4];

AF_DCMotor motorFL(4);
AF_DCMotor motorFR(3);
AF_DCMotor motorBL(1);
AF_DCMotor motorBR(2); 



Servo myservo;//定义舵机变量名

/* const int TrigPin = 8;//设定超声波SR04连接的Arduino引脚
const int EchoPin = 9; */

/* const int KeepAway = 30;//离障碍最近距离
const int speedDownPosition = 60;
const int LookDelay = 300;//转动舵机的停顿时间
const int TurnDelay = 500;//转向停顿时间，通过停顿时间控制转弯角度大小
const int BackDelay = 600;//倒退时间
const int ReboundDelay = 200;//反弹时间 */

int differSpeed = 0;
int defaultSpeed = 250;
int differSpeedDefault=30;

void delayBreak(int delayTime)
{
  for(int i=0;i<(delayTime/5);i++)
  {
    if(!(espSerial.available() > 0))delay(5);
    else break;
    }
 delay(delayTime%5);
 }
void moveF()
{
  motorFL.run(FORWARD); motorFL.setSpeed(pwm[3]); delay(10);
  motorFR.run(BACKWARD);motorFR.setSpeed(pwm[2]);delay(10);
   motorBL.run(FORWARD);motorFL.setSpeed(pwm[0]); delay(10);
  motorBR.run(BACKWARD);motorFL.setSpeed(pwm[1]); delay(10);
 }

void moveL()
{
  motorFL.run(FORWARD); motorFL.setSpeed(pwm[3]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[2]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[0]); 
  motorBR.run(BACKWARD);motorFL.setSpeed(pwm[1]); 
}

void moveR()
{
  motorFL.run(BACKWARD); motorFL.setSpeed(pwm[3]); 
  motorFR.run(BACKWARD);motorFR.setSpeed(pwm[2]);
  motorBL.run(FORWARD);motorFL.setSpeed(pwm[0]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[1]); 
}


void moveB()
{
  motorFL.run(BACKWARD); motorFL.setSpeed(pwm[3]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[2]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[0]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[1]); 
}
void moveFL()
{
  motorFL.run(FORWARD); motorFL.setSpeed(pwm[3]); 
  motorFR.run(RELEASE);//motorFR.setSpeed(pwm[1]);
  motorBL.run(RELEASE);//motorFL.setSpeed(pwm[2]); 
  motorBR.run(FORWARD); motorFL.setSpeed(pwm[1]);  
}

void moveFR()
{
  motorFL.run(RELEASE);// motorFL.setSpeed(pwm[0]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[2]);
  motorBL.run(FORWARD);motorFL.setSpeed(pwm[0]); 
  motorBR.run(RELEASE);//motorFL.setSpeed(pwm[3]); 
}

void moveBL()
{
  motorFL.run(RELEASE); //motorFL.setSpeed(pwm[0]); 
  motorFR.run(BACKWARD);motorFR.setSpeed(pwm[2]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[0]); 
  motorBR.run(RELEASE);//motorFL.setSpeed(pwm[3]); 
}

void moveBR()
{
  motorFL.run(FORWARD); motorFL.setSpeed(pwm[3]); 
  motorFR.run(BACKWARD);motorFR.setSpeed(pwm[2]);
  motorBL.run(FORWARD);motorFL.setSpeed(pwm[0]); 
  motorBR.run(BACKWARD);motorFL.setSpeed(pwm[1]); 
}

void moveStop()
{
  motorFL.run(RELEASE);
  motorFR.run(RELEASE);
  motorBL.run(RELEASE); 
  motorBR.run(RELEASE);
}
#endif
 