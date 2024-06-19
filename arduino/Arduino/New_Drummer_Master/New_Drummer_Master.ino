// PIN DECLARATION
// Inputs
const int pedal_pin = A15;
const int menu_pin = A7;
const int G_pin = A0;
const int R_pin = A1;
const int B_pin = A2;
const int sound_toms_pin[4] = {A10,A11,A12,A13};
const int sound_snare_pin = A8;
const int sound_bass_pin = A9;

// Outputs
const int toms[12] = {2,3,4,5,6,7,8,9,10,11,12,13};
const int toms_green[4] = {2,5,8,11};
const int toms_red[4] = {3,6,9,12};
const int toms_blue[4] = {4,7,10,13};
const int toms_mix[6] = {2,3,4,6,10,11};
const int reflector_pin = A5;
const int black_light_pin = A3;
const int smoke_machine_pin = A4;

// GLOBAL VARIABLES
// These are modificable when testing before selling ------------------------------------
const int gradient_reference = 0;
const int permanent_reference = 1023;
const int stroke_reference = 527;
const int serial_configuration_reference = 352;
const int black_light_reference = 265;
const int fast_live_reference = 212;

const short gradient_case = 0;
const short permanent_case = 1;
const short stroke_case = 2;
const short serial_configuration_case = 3;
const short black_light_case = 4;
const short fast_live_case = 5;

const int menu_reference = 30;    // Delimits the range of tolerance around the references
// --------------------------------------------------------------------------------------

int sound_toms_value[4] = {1023,1023,1023,1023};
int sound_snare_value = 1023;
int sound_bass_value = 1023;
int sound_reference = 50;    // 50 for the minimum

int blink_time = 30;
int menu_value = 0;
int menu_case = 0;

long smoke_time = 4000;   // It's not time, but a counter
long smoke_time_stroke = 2700; // It's different because the features of Stroke Function
long smoke_counter_stroke = 0;  // It needs to be global

// Flags
boolean gradient_break = false;
boolean first_gradient_flag = false;    // Useful to avoid repetition of initial fading
boolean smoke_machine_flag = false;
boolean communication_stablished_flag = false;  // Useful to turn off RGB when Serial_Configuration is set
boolean end_of_fast_live_flag = false;  // Useful to break the entire function while Stroke stage is active

int G_value = 0;
int R_value = 0;
int B_value = 0;
int gradient_counter = 0;

short permanent_code = 0;
short blink_code = 0;
short stroke_code = 0;
short reflector_code = 0;
short menu_option = 0;

short random_color; // It needs to be global, used for Fast_Live

// Libraries
#include "General.h"
#include "Gradient.h"
#include "Serial_Configuration.h"
#include "Permanent.h"
#include "Stroke.h"
#include "Black_Light.h"
#include "Fast_Live.h"

// ================ VOID SETUP ========================
void setup()
{
  Serial.begin(115200);
  // Entradas
  for(int i = 0; i < 4; i++)
    pinMode(sound_toms_pin[i],INPUT);
  pinMode(pedal_pin,INPUT);
  pinMode(sound_snare_pin,INPUT);
  pinMode(sound_bass_pin,INPUT);
  pinMode(G_pin,INPUT);
  pinMode(R_pin,INPUT);
  pinMode(B_pin,INPUT);

  // Salidas
  for(int i = 0; i < 12; i++)
  {
    pinMode(toms[i],OUTPUT);
    digitalWrite(toms[i],LOW);
  }
  pinMode(black_light_pin,OUTPUT);
  pinMode(reflector_pin,OUTPUT);
  pinMode(smoke_machine_pin,OUTPUT);
  digitalWrite(black_light_pin,LOW);
  digitalWrite(reflector_pin,LOW);
  digitalWrite(smoke_machine_pin,LOW);

  randomSeed(analogRead(A14));    // In order to create a real random, A14 is arbitrary
} 

void loop()
{
  //Sensors_Testing();
  goto MENU;
  MENU:
  {
    Menu_Lecture();
    switch(menu_case)
    {
      case 0:
        //Serial.println("goto GRADIENT");
        goto GRADIENT;
        break;
      case 1:
        //Serial.println("goto PERMANENT");
        goto PERMANENT;
        break;
      case 2:
        goto STROKE;
        break;
      case 3:
        //Serial.println("goto SERIAL_CONFIGURATION");
        goto SERIAL_CONFIGURATION;
        break;
      case 4:
        //Serial.println("goto BLACK_LIGHT");
        goto BLACK_LIGHT;
        break;
      case 5:
        //Serial.println("goto FAST_LIVE");
        goto FAST_LIVE;
        break;
      default:
        goto MENU;
        break;
    }
  }

  GRADIENT:
  {
    Gradient();
    goto MENU;
  }

  PERMANENT:
  {
    Permanent();
    goto MENU;
  }

  STROKE:
  {
    Stroke();
    goto MENU;
  }

  SERIAL_CONFIGURATION:
  {
    Serial_Configuration();
    goto MENU;
  }
  
  BLACK_LIGHT:
  {
    Black_Light();
    goto MENU;
  }

  FAST_LIVE:
  {
    Fast_Live();
  }
}

