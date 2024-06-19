void Fast_Live()
{
  // Turns off all statements before sequence
  digitalWrite(black_light_pin,LOW);
  digitalWrite(reflector_pin,LOW);
  digitalWrite(smoke_machine_pin,LOW);
  for(int i = 0; i < 12; i++)
    digitalWrite(toms[i],LOW);
  
  // First Stage: Mix Gradient Rising ---------------------------------------------
  for(int j = 0; j < 255; j++)
  {
    for(int i = 0; i < 6; i++)
    {
      analogWrite(toms_mix[i],j);

      // Menu Lecture always
      Menu_Lecture();
      if((menu_case != fast_live_case))
        goto BREAK;
    }
    delay(10);
  }
  
  // ------------------------------------------------------------------------------

  // Second Stage: Permanent Mix ==================================================
  for(int j = 0; j < 1000; j++)
  {
    for(int i = 0; i < 6; i++)
    {
      digitalWrite(toms_mix[i],HIGH);

      // Menu Lecture always
      Menu_Lecture();
      if((menu_case != fast_live_case))
        goto BREAK;
    }
    delay(20);
  }
  for(int j = 255; j > 0; j--)
    {
      for(int i = 0; i < 6; i++)
      {
        analogWrite(toms_mix[i],j);
  
        // Menu Lecture always
        Menu_Lecture();
        if((menu_case != fast_live_case))
          goto BREAK;
      }
      delay(10);
    }
  // ===============================================================================
  
  // Third Stage: Random Blink & Smoke at the end * * * * * * * * * * * * * * * * * *
  Blink(70);
  Blink(40);
  Blink(80);
  Blink(30);
  Blink(50);
  Blink(100);
  Blink(70);
  digitalWrite(black_light_pin,HIGH);
  digitalWrite(smoke_machine_pin,HIGH);
  for(long smoke_counter = 0; smoke_counter < smoke_time; smoke_counter++)
  {
    Menu_Lecture();
    if(menu_case != fast_live_case)
      goto BREAK;
    delay(2);
  }
  digitalWrite(smoke_machine_pin,LOW);
  digitalWrite(black_light_pin,LOW);
  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  // Fourth Stage: Reflector ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  digitalWrite(reflector_pin,HIGH);
  for(int i = 0; i < 200; i++)
  {
    Menu_Lecture();
    if(menu_case != fast_live_case)
      goto BREAK;
    delay(50);
  }
  digitalWrite(reflector_pin,LOW);
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // Fifth Stage: Black Light & Stroke /////////////////////////////////////////////
  digitalWrite(black_light_pin,HIGH);
  for(long i = 0; i < 10000; i++)
  {
    Stroke();
    // If inside Stroke I change the Menu, broke the entire function
    if(end_of_fast_live_flag == true)
    {
      end_of_fast_live_flag = false;
      goto BREAK;
    }
  }
  digitalWrite(black_light_pin,LOW);
  // ///////////////////////////////////////////////////////////////////////////////

  // Sixth and Final Stage: Random Permanent & Reflector & Smoke %%%%%%%%%%%%%%%%%%%
  random_color = random(1,6);
  digitalWrite(reflector_pin,HIGH);
  switch(random_color)
  {
    case 1:   // Green
      for(int j = 0; j < 1000; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(40);
      }
      for(int i = 0; i < 4; i++)
        digitalWrite(toms_green[i],LOW);
      break;
    case 2:   // Red
      for(int j = 0; j < 1000; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_red[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(40);
      }
      for(int i = 0; i < 4; i++)
        digitalWrite(toms_red[i],LOW);
      break;
    case 3:   // Blue
      for(int j = 0; j < 1000; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_blue[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(40);
      }
      for(int i = 0; i < 4; i++)
        digitalWrite(toms_blue[i],LOW);
      break;
    case 4:   // Cyan
      for(int j = 0; j < 1000; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          digitalWrite(toms_blue[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(40);
      }
      for(int i = 0; i < 4; i++)
      {
        digitalWrite(toms_green[i],LOW);
        digitalWrite(toms_blue[i],LOW);
      }
      break;
    case 5:   // Yellow
      for(int j = 0; j < 1000; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          digitalWrite(toms_red[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(40);
      }
      for(int i = 0; i < 4; i++)
      {
        digitalWrite(toms_green[i],LOW);
        digitalWrite(toms_red[i],LOW);
      }
      break;
    case 6:   // Pink
      for(int j = 0; j < 1000; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_red[i],HIGH);
          digitalWrite(toms_blue[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(40);
      }
      for(int i = 0; i < 4; i++)
      {
        digitalWrite(toms_red[i],LOW);
        digitalWrite(toms_blue[i],LOW);
      }
      break;
  }
  digitalWrite(smoke_machine_pin,HIGH);
  for(long smoke_counter = 0; smoke_counter < smoke_time; smoke_counter++)
  {
    Menu_Lecture();
    if(menu_case != fast_live_case)
      goto BREAK;
    delay(2);
  }
  digitalWrite(smoke_machine_pin,LOW);
  digitalWrite(reflector_pin,LOW);
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
  BREAK:
  {Serial.println("FAST_LIVE BROKEN");}
}
