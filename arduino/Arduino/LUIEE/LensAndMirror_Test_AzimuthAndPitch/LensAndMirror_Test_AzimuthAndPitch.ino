const int LM_pin[8] = {22,24,26,28,30,32,34,36};      // {Rstp1_dir,Rstp1_step,Pstp2_dir,Pstp2_step,PMstp3_1,PMstp3_2,PMstp3_3,PMspt3_4}

int LM_data[3] = {10,10,0};

/* Number of full steps per revolution: 200*/
unsigned long timer_prev1 = 0;
unsigned long timer1 = 0;
unsigned long timer_difference1 = 0;
boolean activated_flag1 = false;
int miliSeconds_stepper1 = 100;   // min 10, max 100
int counter_steps1 = 0;
float current_stepper_angle1 = 0;

/* Number of full steps per revolution: 200*/
unsigned long timer_prev2 = 0;
unsigned long timer2 = 0;
unsigned long timer_difference2 = 0;
boolean activated_flag2 = false;
int miliSeconds_stepper2 = 100;   // min 10, max 100
int counter_steps2 = 0;
float current_stepper_angle2 = 0;

void setup()
{
  Serial.begin(57600);
  for(int i = 0; i < 8; i++)
    pinMode(LM_pin[i], OUTPUT);
  pinMode(13,OUTPUT);
  
  delay(1000);
  digitalWrite(13,HIGH);
}

void loop()
{
    timer_prev1 = millis();
    timer_prev2 = millis();
    while(1)
    {      
      if(counter_steps1 <= abs(LM_data[0]))
      {
        // Change in direction
        if(LM_data[0] > 0)
          digitalWrite(LM_pin[0],HIGH);
        else
          digitalWrite(LM_pin[0],LOW);
        
        timer1 = millis();
        timer_difference1 = timer1 - timer_prev1;
    
        // Writing the stepper power considering "delayMicroSeconds" with micros()
        if(activated_flag1 == false)
        {
          if(timer_difference1 < miliSeconds_stepper1)
          {
            Serial.println("ACTIVATED");
            digitalWrite(LM_pin[1],HIGH);
          }
          else
          {
            activated_flag1 = true;
            counter_steps1++;
            if(LM_data[0] > 0)
              current_stepper_angle1 += 1.8;
            else
              current_stepper_angle1 -= 1.8;
          }
            
        }
        if(activated_flag1 == true)
        {
          if(timer_difference1 > miliSeconds_stepper1 && timer_difference1 < miliSeconds_stepper1*2)
          {
            Serial.println("DEACTIVATED");
            digitalWrite(LM_pin[1],LOW);
          }
          else
          {
            activated_flag1 = false;
            break;
          }
        }  
      }
      else
        digitalWrite(LM_pin[1],LOW);

      if(counter_steps2 <= abs(LM_data[1]))
      {
        // Change in direction
        if(LM_data[1] > 0)
          digitalWrite(LM_pin[2],HIGH);
        else
          digitalWrite(LM_pin[2],LOW);
        
        timer2 = millis();
        timer_difference2 = timer2 - timer_prev2;
    
        // Writing the stepper power considering "delayMicroSeconds" with micros()
        if(activated_flag2 == false)
        {
          if(timer_difference2 < miliSeconds_stepper2)
          {
            Serial.println("ACTIVATED");
            digitalWrite(LM_pin[3],HIGH);
          }
          else
          {
            activated_flag2 = true;
            counter_steps2++;
            if(LM_data[1] > 0)
              current_stepper_angle2 += 1.8;
            else
              current_stepper_angle2 -= 1.8;
          }
            
        }
        if(activated_flag2 == true)
        {
          if(timer_difference2 > miliSeconds_stepper2 && timer_difference2 < miliSeconds_stepper2*2)
          {
            Serial.println("DEACTIVATED");
            digitalWrite(LM_pin[3],LOW);
          }
          else
          {
            activated_flag2 = false;
            break;
          }
        }  
      }
      else
        digitalWrite(LM_pin[3],LOW);
    }
}
