#include <Stepper.h>

/* Pins for the driver control */
const int LM_pin[2] = {28,30};

int LM_data = 200;   // min 10, max a lot! around 30000, suitable between 10 and 30!

unsigned long timer_prev = 0;
unsigned long timer = 0;
unsigned long timer_difference = 0;

boolean activated_flag = false;
int miliSeconds_stepper = 10;   // min 10, max 500
int counter_steps = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(LM_pin[0],OUTPUT);
  pinMode(LM_pin[1],OUTPUT);
}

//void Adjust_azimuth(int Steps, byte Direction) {
void loop()
{
  timer_prev = millis();
  counter_steps++;
  while(1)
  {
    if(counter_steps <= LM_data)
    {
      // Change in direction
      if(LM_data > 0)
        digitalWrite(LM_pin[0],HIGH);
      else
        digitalWrite(LM_pin[0],LOW);
      
      timer = millis();
      timer_difference = timer - timer_prev;
  
      // Writing the stepper power considering "delayMicroSeconds" with micros()
      if(activated_flag == false)
      {
        if(timer_difference < miliSeconds_stepper)
        {
          Serial.println("ACTIVATED");
          digitalWrite(LM_pin[1],HIGH);
        }
        else
          activated_flag = true;
      }
      if(activated_flag == true)
      {
        if(timer_difference > miliSeconds_stepper && timer_difference < miliSeconds_stepper*2)
        {
          Serial.println("DEACTIVATED");
          digitalWrite(LM_pin[1],LOW);
        }
        else
        {
          activated_flag = false;
          break;
        }
      }  
    }
    else
      digitalWrite(LM_pin[1],LOW);
  }
}
