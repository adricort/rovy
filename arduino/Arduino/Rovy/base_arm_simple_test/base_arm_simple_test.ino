#include <SoftwareSerial.h>

const int top_mot_forw = 3;
const int top_mot_back = 5;
const int bot_mot_forw = 6;
const int bot_mot_back = 9;
const int LED = 13;
char msg = 0;
int pwm = 0;
#define BTTX 10
#define BTRX 11
SoftwareSerial SerialBT(BTTX, BTRX);

void setup() {
  SerialBT.begin(9600);
  Serial.begin(57600);
  pinMode(top_mot_forw, OUTPUT);
  pinMode(top_mot_back, OUTPUT);
  pinMode(bot_mot_forw, OUTPUT);
  pinMode(bot_mot_back, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

void forwards(int pwm)
{
  analogWrite(top_mot_forw, pwm);
  analogWrite(top_mot_back, 0);
  analogWrite(bot_mot_forw, pwm);
  analogWrite(bot_mot_back, 0);
}

void backwards(int vel)
{
  analogWrite(top_mot_forw, 0);
  analogWrite(top_mot_back, pwm);
  analogWrite(bot_mot_forw, 0);
  analogWrite(bot_mot_back, pwm);
}

void stop_motors()
{
  analogWrite(top_mot_forw, 0);
  analogWrite(top_mot_back, 0);
  analogWrite(bot_mot_forw, 0);
  analogWrite(bot_mot_back, 0);
}

void loop() {
  if(SerialBT.available() > 0)
  {
    String msg = SerialBT.readString();
    pwm = msg.toInt();
    if (pwm > 0)
      forwards(pwm);
    else if (pwm < 0)
    {
      pwm = pwm*-1;
      backwards(pwm);
    }
    else if (pwm == 0)
      stop_motors();
    Serial.println(pwm);
  }
}
