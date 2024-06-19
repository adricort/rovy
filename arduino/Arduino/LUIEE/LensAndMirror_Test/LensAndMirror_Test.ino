const int LM_pin[8] = {22,24,26,28,30,32,34,36};      // {Rstp1_dir,Rstp1_step,Pstp2_dir,Pstp2_step,PMstp3_1,PMstp3_2,PMstp3_3,PMspt3_4}

int LM_data[3] = {60,0,0};   // in deg
#define k 1.8

unsigned long timer_hs_prev = 0;
unsigned long timer_hs = 0;
unsigned long timer_hs_difference = 0;
boolean activated_hs_flag = false;
int miliSeconds_hs_stepper = 100;   // min 10, max 200
int counter_hs_steps = 0;
float current_hs_angle = 0;
boolean miliSeconds_reached_flag = false;

void setup()
{
  Serial.begin(57600);
  for(int i = 0; i < 8; i++)
    pinMode(LM_pin[i], OUTPUT);
  pinMode(13,OUTPUT);
  
  delay(1000);
  digitalWrite(13,HIGH);
  timer_hs_prev = millis();
}

void loop()
{
  // Change in direction
  if(LM_data[0] > 0)
    digitalWrite(LM_pin[0],HIGH);
  else
    digitalWrite(LM_pin[0],LOW);
    
  if(miliSeconds_reached_flag == true)
  {
    timer_hs_prev = millis();
    miliSeconds_reached_flag = false;
  }      

  int desired_hs_steps = int(abs(LM_data[0])/k);
  if(counter_hs_steps < desired_hs_steps)
  {
    timer_hs = millis();
    timer_hs_difference = timer_hs - timer_hs_prev;

    if(activated_hs_flag == false)
    {
      if(timer_hs_difference >= miliSeconds_hs_stepper)
      {
        activated_hs_flag = true;
        counter_hs_steps++;
        timer_hs_prev = millis();
      }
      else
        digitalWrite(LM_pin[1],HIGH);   
    }
    if(LM_data[0] > 0)
      current_hs_angle = counter_hs_steps*k;
    else
      current_hs_angle = -counter_hs_steps*k;;
   
    timer_hs = millis();
    timer_hs_difference = timer_hs - timer_hs_prev;
    
    if(activated_hs_flag == true)
    {
      if(timer_hs_difference >= miliSeconds_hs_stepper)
      {
        activated_hs_flag = false;
        miliSeconds_reached_flag = true;
      }
      else
        digitalWrite(LM_pin[1],LOW);
    }  
  }
}
