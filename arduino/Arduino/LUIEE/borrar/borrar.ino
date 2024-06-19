  // Libraries:
  #include <Wire.h>
  /* Library for the small stepper motor. Used to control the position of the plane mirror */
  #include <Stepper.h>
  /* Add library and create an instance of the sensor object */
  #include <FaBo9Axis_MPU9250.h>
  FaBo9Axis fabo_9axis;         //  CONSIDER MAKING THE OBJECT LOCAL
  
  #include <ros.h>
  #include <std_msgs/Int32MultiArray.h>
  #include <std_msgs/Float32MultiArray.h>
  
  /*  Order of Modes: Nav_Mode,Cent_Mode,Extr_Mode,Stor_Mode,Rec_Mode,Man_Mode,Stan_Mode,Safe_Mode
   *  
   */
    const int LM_pin[8] = {22,24,28,26,30,32,34,36};      // {Rstp1_dir,Rstp1_step,Pstp2_dir,Pstp2_step,PMstp3_1,PMstp3_2,PMstp3_3,PMspt3_4}
  /*
  const int temp_sens_pin[2] = {A0, A1};
  const int voltage_sens_pin[2] = {A2, A3};
  const int load_sens_pin[3] = {A4, A5, A6};
  const int water_lvl_sens_pin = A7;
  const int phototrans_pin[8] = {A8, A9, A10, A11, A12, A13, A14, A15};*/
  
  // ------------------------ Variables definition ---------------------------

  int LM_data[3] = {50,0,0};


  
/* Number of full steps per revolution: 200*/
unsigned long timer_prev = 0;
unsigned long timer = 0;
unsigned long timer_difference = 0;
boolean activated_flag = false;
int miliSeconds_stepper = 100;   // min 10, max 100
int counter_steps = 0;
float current_stepper_angle = 0;
  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  

  void setup()
  {
    Serial.begin(57600);

 
    for(int i = 0; i < 8; i++)
      pinMode(LM_pin[i], OUTPUT);

  }
  
  void loop()
  {
    timer_prev = millis();
    while(1)
    {    
     
      /*switch(LEDring_data)       PENDING TO BE ADAPTED TO THE LEDring
      {
        case 0:
          for(int i = 0; i < 3; i++)
            digitalWrite(LED_pin, LOW);
          break;
        case 1:
          digitalWrite(LED_pin[0], HIGH);
          digitalWrite(LED_pin[1], LOW);
          digitalWrite(LED_pin[2], LOW);
          break;
        case 2:
          digitalWrite(LED_pin[0], LOW);
          digitalWrite(LED_pin[1], HIGH);
          digitalWrite(LED_pin[2], LOW);
          break;
        case 3:
          digitalWrite(LED_pin[0], LOW);
          digitalWrite(LED_pin[1], LOW);
          digitalWrite(LED_pin[2], HIGH);
          break;
        case 4:
          digitalWrite(LED_pin[0], HIGH);
          digitalWrite(LED_pin[1], HIGH);
          digitalWrite(LED_pin[2], HIGH);
          break;
        case 5:
          digitalWrite(LED_pin[0], HIGH);
          digitalWrite(LED_pin[1], LOW);
          digitalWrite(LED_pin[2], HIGH);
          break;
        default:
          break;
      }
    
      if(linActs_pwm > 0)
      {
        linAct_vapChamb_data[0] = linActs_pwm;
        linAct_vapChamb_data[1] = 0;
      }
      else
      {
        linAct_vapChamb_data[0] = 0;
        linAct_vapChamb_data[1] = abs(linActs_pwm);
      }
      
      analogWrite(linAct_vapChamb_pin[0], linAct_vapChamb_data[0]);
      analogWrite(linAct_vapChamb_pin[1], linAct_vapChamb_data[1]);*/
  
      if(counter_steps <= abs(LM_data[0]))
      {
        // Change in direction
        if(LM_data[0] > 0)
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
          {
            activated_flag = true;
            counter_steps++;
            if(LM_data > 0)
              current_stepper_angle += 1.8;
            else
              current_stepper_angle -= 1.8;
          }
            
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
