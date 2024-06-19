#include <Stepper.h>

#define STEPS 2038
#define k 5.66
#define SPEED 5   // recommendation between 1 and 15 [min, max]
Stepper stepper(STEPS, 36, 34, 30, 32);

const int LM_pin[8] = {22,24,26,28,30,32,34,36};      // {Rstp1_dir,Rstp1_step,Pstp2_dir,Pstp2_step,PMstp3_1,PMstp3_2,PMstp3_3,PMspt3_4}

int LM_data[3] = {0,0,180};

/* Number of full steps per revolution: 200*/
unsigned long timer_prev_pm = 0;
unsigned long timer_pm = 0;
unsigned long timer_difference_pm = 0;
int counter_steps_pm = 0;
int desired_steps_pm = 0;
float current_pm_angle = 0;
float desired_pm_angle = 0;

void setup()
{
  Serial.begin(57600);
  for(int i = 0; i < 8; i++)
    pinMode(LM_pin[i], OUTPUT);
  pinMode(13,OUTPUT);

  // set the speed of the motor to SPEED RPMs
  stepper.setSpeed(SPEED); // FOR THE PLANE MIRROR
  
  delay(1000);
  digitalWrite(13,HIGH);
}

void loop()
{
  if(LM_data[2] > 0)
  {
    stepper.step(1);
    counter_steps_pm++;
    desired_steps_pm = int(LM_data[2]*k);
    if(counter_steps_pm >= desired_steps_pm)
    {
      stepper.step(0);
      while(1){};
    }
  }
  else
  {
    stepper.step(-1);
    counter_steps_pm++;
    desired_steps_pm = abs(int(LM_data[2]*k));
    if(counter_steps_pm >= desired_steps_pm)
    {
      stepper.step(0);
      while(1){};
    }
  }
}
