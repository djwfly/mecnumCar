#ifndef MOTORACTION_H
#define MOTORACTION_H

#include <AFMotor.h>

extern int pwm[4];

AF_DCMotor motorFL(4);
AF_DCMotor motorFR(3);
AF_DCMotor motorBL(1);
AF_DCMotor motorBR(2); 


Servo myservo;//定义舵机变量名

const int TrigPin = 8;//设定超声波SR04连接的Arduino引脚
const int EchoPin = 9;

const int KeepAway = 30;//离障碍最近距离
const int speedDownPosition = 60;
const int LookDelay = 300;//转动舵机的停顿时间
const int TurnDelay = 500;//转向停顿时间，通过停顿时间控制转弯角度大小
const int BackDelay = 600;//倒退时间
const int ReboundDelay = 200;//反弹时间

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
  motorFL.run(FORWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(BACKWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(FORWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(BACKWARD);motorFL.setSpeed(pwm[3]); 
}

void moveL()
{
  motorFL.run(FORWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(BACKWARD);motorFL.setSpeed(pwm[3]); 
}

void moveR()
{
  motorFL.run(BACKWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(BACKWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(FORWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[3]); 
}


void moveB()
{
  motorFL.run(BACKWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[3]); 
}
void moveFL()
{
/*   motorFL.run(BACKWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[3]);  */
}

void moveFR()
{
/*   motorFL.run(BACKWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[3]);  */
}

void moveBL()
{
/*   motorFL.run(BACKWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[3]);  */
}

void moveBR()
{
/*   motorFL.run(BACKWARD); motorFL.setSpeed(pwm[0]); 
  motorFR.run(FORWARD);motorFR.setSpeed(pwm[1]);
  motorBL.run(BACKWARD);motorFL.setSpeed(pwm[2]); 
  motorBR.run(FORWARD);motorFL.setSpeed(pwm[3]);  */
}

void moveStop()
{
  motorFL.run(RELEASE);
  motorFR.run(RELEASE);
  motorBL.run(RELEASE); 
  motorBR.run(RELEASE);
}


void goForward(int i)
{
  motorFL.run(FORWARD);motorFL.setSpeed(i); 
  motorFR.run(BACKWARD);motorFR.setSpeed(i-differSpeed);
}

void goBack(int i)
{
motorFL.run(BACKWARD);motorFL.setSpeed(i); 
motorFR.run(FORWARD);motorFR.setSpeed(i-differSpeed);
}

void goStop(int i)
{
motorFL.run(RELEASE);
motorFR.run(RELEASE);
}

void turnRight(int i)
{
motorFL.run(BACKWARD);motorFL.setSpeed(i); 
motorFR.run(BACKWARD);motorFR.setSpeed(i-differSpeed);
}

void turnLeft(int i)
{
motorFL.run(FORWARD);motorFL.setSpeed(i); 
motorFR.run(FORWARD);motorFR.setSpeed(i-differSpeed);
}

void goRebound(int i)
{
motorFL.run(BACKWARD);motorFL.setSpeed(i); 
motorFR.run(FORWARD);motorFR.setSpeed(i-differSpeed);
  delayBreak(ReboundDelay);
  goStop(0);
}

void lookRight1() {//向右看90度
  myservo.write(5);
  delayBreak(LookDelay);
}
void lookRight2() {//向右看45度
  myservo.write(45);
  delayBreak(LookDelay);
}
void lookForward() {//向右看前
  myservo.write(90);
  delayBreak(LookDelay);
}
void lookLeft4() {//向左看45度
  myservo.write(135);
  delayBreak(LookDelay);
}
void lookLeft5() {//向左看90度
  myservo.write(175);
  delayBreak(LookDelay);
}
float checkDistance(int TrigPin, int EchoPin) {//超声波探测障碍物距离函数
  float distance;
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  // 检测脉冲宽度，并计算出距离
  distance = pulseIn(EchoPin, HIGH) / 58.00;
  return distance;
}
int lookAround() {//观察周围情况，并返回方向代码
  lookRight1();
  int D1 = checkDistance(TrigPin, EchoPin);
  lookRight2();
  int D2 = checkDistance(TrigPin, EchoPin);
  lookForward();
  int D3 = checkDistance(TrigPin, EchoPin);
  lookLeft4();
  int D4 = checkDistance(TrigPin, EchoPin);
  lookLeft5();
  int D5 = checkDistance(TrigPin, EchoPin);
  if (D1 > D2 && D1 > D3 && D1 > D4 && D1 > D5 && D1 > KeepAway)return 1;
  if (D2 > D1 && D2 > D3 && D2 > D4 && D2 > D5 && D2 > KeepAway)return 2;
  if (D3 > D2 && D3 > D1 && D3 > D4 && D3 > D5 && D3 > KeepAway)return 3;
  if (D4 > D2 && D4 > D3 && D4 > D1 && D4 > D5 && D4 > KeepAway)return 4;
  if (D5 > D2 && D5 > D3 && D5 > D4 && D5 > D1 && D5 > KeepAway)return 5;
  return 0;
}
#endif
