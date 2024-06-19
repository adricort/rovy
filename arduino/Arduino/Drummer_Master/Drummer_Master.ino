// 

// Pin declaration
const int toms[12] = {40,42,44,49,45,47,38,36,34,46,48,50};
const int toms_green[4] = {40,49,38,46};
const int toms_red[4] = {42,45,36,48};
const int toms_blue[4] = {44,47,34,50};
const int toms_mix[6] = {40,47,38,48};
const int pedal_pin = 3;
const int reflector_pin = 4;
const int black_pin = 5;

const int sensor_toms[5] = {A12,A15,A14,A13};
const int sensor_snare = A11;
const int sensor_bass = 2;

// Global Variables
int sensor_toms_value[4] = {1023,1023,1023,1023};
int sensor_snare_value = 1023;
int sensor_bass_value = 1023;

int blink_time = 30;
boolean pedal_flag = false;
boolean pedal_value = false;
boolean pedal_last_value = false;

short permanent_code = 0;
short blink_code = 0;
short stroke_code = 0;
short reflector_code = 0;

// Libraries
#include "General.h"
#include "Blinks.h"
#include "Permanents.h"
#include "Strokes.h"

// ================ VOID SETUP ========================
void setup()
{
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  pinMode(pedal_pin,INPUT);
  pinMode(black_pin,OUTPUT);
  pinMode(reflector_pin,OUTPUT);
  pinMode(sensor_snare,INPUT);
  pinMode(sensor_bass,INPUT);
  
  for(int i = 0; i < 12; i++)
  {
    pinMode(toms[i],OUTPUT);
    digitalWrite(toms[i],LOW);
  }
  for(int i = 0; i < 4; i++)
  {
    pinMode(sensor_toms[i],INPUT);
  }
} 

// =============== VOID LOOP =========================
void loop() 
{/*
  if(Serial.available() > 0) // Checks whether data is comming from the serial port
  {
    Turn_Off();
    String configuration = Serial.readStringUntil(36);  // 36 = $

    int indice_0 = configuration.lastIndexOf('p');
    int indice_1 = configuration.indexOf('b',indice_0+1);
    int indice_2 = configuration.indexOf('s',indice_1+1);
    int indice_3 = configuration.indexOf('r',indice_2+1);
    int indice_4 = configuration.indexOf('a',indice_3+1);

    String number_1 = configuration.substring(indice_0+1,indice_1);
    String number_2 = configuration.substring(indice_1+1,indice_2);
    String number_3 = configuration.substring(indice_2+1,indice_3);
    String number_4 = configuration.substring(indice_3+1,indice_4);

    permanent_code = number_1.toInt();
    blink_code = number_2.toInt();
    stroke_code = number_3.toInt();
    reflector_code = number_4.toInt();

    Serial.print("Permanent: ");Serial.print(permanent_code);
    Serial.print(" Blink: ");Serial.print(blink_code);
    Serial.print(" Stroke: ");Serial.print(stroke_code);
    Serial.print(" Reflector: ");Serial.println(reflector_code);
  }

  switch(reflector_code)
  {
    case 0:
      Turn_Off_Reflector();
      break;
    case 1:
      Permanent_Reflector();
      break;
    case 2:
      Turn_Off_Reflector();
      attachInterrupt(digitalPinToInterrupt(sensor_bass), Stroke_Bass, RISING);
      break;
  }
  
  switch(permanent_code)
  {
    case 0:
      Turn_Off();
      digitalWrite(black_pin,LOW);
      break;
    case 1:
      Permanent_Green();
      break;
    case 2:
      Permanent_Red();
      break;
    case 3:
      Permanent_Blue();
      break;
    case 4:
      Permanent_Cyan();
      break;
    case 5:
      Permanent_Pink();
      break;
    case 6:
      Permanent_Yellow();
      break;
    case 7:
      Permanent_White();
      break;
    case 8:
      Permanent_Mix();
      break;
    case 9:
      Permanent_Black();
      break;
  }

  switch(blink_code)
  {
    case 0:
      break;
    case 1:
      Blink_Green(0);
      break;
    case 2:
      Blink_Red(0);
      break;
    case 3:
      Blink_Blue(0);
      break;
    case 4:
      Blink_Cyan(0);
      break;
    case 5:
      Blink_Pink(0);
      break;
    case 6:
      Blink_Yellow(0);
      break;
    case 7:
      Blink_White(0);
      break;
    case 8:
      Blink_Mix(0);
      break;
    case 9:
      Blink_Green(1);
      break;
    case 10:
      Blink_Red(1);
      break;
    case 11:
      Blink_Blue(1);
      break;
    case 12:
      Blink_Cyan(1);
      break;
    case 13:
      Blink_Pink(1);
      break;
    case 14:
      Blink_Yellow(1);
      break;
    case 15:
      Blink_White(1);
      break;
    case 16:
      Blink_Mix(1);
      break;
  }

  switch(stroke_code)
  {
    case 0:
      break;
    case 1:
      Stroke_Green();
      break;
    case 2:
      Stroke_Red();
      break;
    case 3:
      Stroke_Blue();
      break;
    case 4:
      Stroke_Cyan();
      break;
    case 5:
      Stroke_Pink();
      break;
    case 6:
      Stroke_Yellow();
      break;
    case 7:
      Stroke_White();
      break;
  }*/

  
  STATE_1:
  {
    //Blink_Mix(0);
    Permanent_Black();
    attachInterrupt(digitalPinToInterrupt(sensor_bass), Stroke_Bass, RISING);
    Stroke_Cyan();
    
    if(pedal_flag == true)
    {
      Turn_Off();
      pedal_flag = false;
      //goto STATE_2; 
    }
    else
    {
      goto STATE_1;
    }
  }
/*
  STATE_2:
  {
    Blink_Green(1);
    Turn_Off_Reflector();
    
    if(pedal_flag == true)
    {
      Turn_Off();
      pedal_flag = false;
      goto STATE_3; 
    }
    else
    {
      goto STATE_2;
    }
  }

  STATE_3:
  {
    Blink_Blue(0);
    Turn_Off_Reflector();
    
    if(pedal_flag == true)
    {
      Turn_Off();
      pedal_flag = false;
      goto STATE_4; 
    }
    else
    {
      goto STATE_3;
    }
  }

  STATE_4:
  {
    Permanent_Black();
    Stroke_Cyan();
    attachInterrupt(digitalPinToInterrupt(sensor_bass), Stroke_Bass, RISING);

    if(pedal_flag == true)
    {
      Turn_Off();
      pedal_flag = false;
      goto STATE_5; 
    }
    else
    {
      goto STATE_4;
    }
  }

  STATE_5:
  {
    Permanent_Red();
    Turn_Off_Reflector();

    if(pedal_flag == true)
    {
      Turn_Off();
      pedal_flag = false;
      goto STATE_1; 
    }
    else
    {
      goto STATE_5;
    }
  }*/
}

