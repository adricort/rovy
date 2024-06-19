/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/
int command = 0;
const int servos_pins[5] = {9,10,11,12,A0};
const int origin_servos[5] = {15,15,15,165,165};
const int final_servos[5] = {180,180,160,20,0};
const int flex_pins[5] = {A5,A4,A3,A2,A1};
int flex_values[5] = {0,0,0,0,0};
int servos_values[5] = {0,0,0,180,180};

#include <Servo.h>

Servo servos[5];

void setup() {
  pinMode(13,OUTPUT);
  for(int i = 0; i < 5; i++)
  {
    servos[i].attach(servos_pins[i]);
    servos[i].write(origin_servos[i]);
    pinMode(flex_pins[i],INPUT);
  }
  digitalWrite(13,HIGH);
}

void loop() {
  for(int i = 0; i < 5; i++)
  {
    flex_values[i] = analogRead(flex_pins[i]);
    if(flex_values[i] >= 150)
    {
      servos_values[i] = final_servos[i];
      digitalWrite(13,LOW);
      delay(15);
    }
    else
    {
      digitalWrite(13,HIGH);
      servos_values[i] = origin_servos[i];
      delay(15);
    }
    servos[i].write(servos_values[i]);
  }
}

