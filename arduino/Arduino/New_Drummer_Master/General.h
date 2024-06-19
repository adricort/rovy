void Menu_Lecture()
{
  menu_value = analogRead(menu_pin);
  //Serial.print("menu_value: ");Serial.print(menu_value);Serial.print("... ");
  if(menu_value <= (gradient_reference + menu_reference))
  {
    menu_case = gradient_case;
  }
  else if(menu_value >= (permanent_reference - menu_reference))
  {
    menu_case = permanent_case;
  }
  else if(menu_value >= (stroke_reference - menu_reference) && menu_value <= (stroke_reference + menu_reference))
  {
    menu_case = stroke_case;
  }
  if(menu_value >= (serial_configuration_reference - menu_reference) && menu_value <= (serial_configuration_reference + menu_reference))
  {
    menu_case = serial_configuration_case;
  }
  else if(menu_value >= (black_light_reference - menu_reference) && menu_value <= (black_light_reference + menu_reference))
  {
    menu_case = black_light_case;
  }
  else if(menu_value >= (fast_live_reference - menu_reference) && menu_value <= (fast_live_reference + menu_reference))
  {
    menu_case = fast_live_case;
  }

  if(menu_case != 3)
    communication_stablished_flag = false;
}

void Smoke_Machine(int configuration_case)
{
  // Pedal pin for smoke machine:
  if(digitalRead(pedal_pin))
  {
    digitalWrite(smoke_machine_pin,HIGH);
    for(long smoke_counter = 0; smoke_counter < smoke_time; smoke_counter++)
    {
      //Serial.println(smoke_counter);  // Don't uncomment this, it affects the time
      Menu_Lecture();
      if(menu_case != configuration_case)
        smoke_counter = smoke_time;
      delay(2);
    }
    digitalWrite(smoke_machine_pin,LOW);
  }
  else
    digitalWrite(smoke_machine_pin,LOW);
}

void Blink(short velocity)
{
  random_color = random(1,6);
  switch(random_color)
  {
    case 1:   // Green
      for(int j = 0; j < 60; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(velocity);
        for(int i = 0; i < 4; i++)
          digitalWrite(toms_green[i],LOW);
        delay(velocity);
      }
      break;
    case 2:   // Red
      for(int j = 0; j < 60; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_red[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(velocity);
        for(int i = 0; i < 4; i++)
          digitalWrite(toms_red[i],LOW);
        delay(velocity);
      }
      break;
    case 3:   // Blue
      for(int j = 0; j < 60; j++)
      {
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_blue[i],HIGH);
          // Menu Lecture always
          Menu_Lecture();
          if((menu_case != fast_live_case))
            break;
        }
        delay(velocity);
        for(int i = 0; i < 4; i++)
          digitalWrite(toms_blue[i],LOW);
        delay(velocity);
      }
      break;
    case 4:   // Cyan
      for(int j = 0; j < 60; j++)
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
        delay(velocity);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],LOW);
          digitalWrite(toms_blue[i],LOW);  
        }
        delay(velocity);
      }
      break;
    case 5:   // Yellow
      for(int j = 0; j < 60; j++)
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
        delay(velocity);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],LOW);
          digitalWrite(toms_red[i],LOW);  
        }
        delay(velocity);
      }
      break;
    case 6:   // Pink
      for(int j = 0; j < 60; j++)
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
        delay(velocity);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_red[i],LOW);
          digitalWrite(toms_blue[i],LOW);  
        }
        delay(velocity);
      }
      break;
  }
}

// Only for testing before selling
void Sensors_Testing()
{
  //Serial.print("Potentiometers: ");Serial.print(analogRead(G_pin));
  //Serial.print(" ");Serial.print(analogRead(R_pin));
  //Serial.print(" ");Serial.print(analogRead(B_pin));

  /*Serial.print("Sound sensors: ");Serial.print(analogRead(sound_toms_pin[0]));Serial.print(" ");
  Serial.print(analogRead(sound_toms_pin[1]));Serial.print(" ");
  Serial.print(analogRead(sound_toms_pin[2]));Serial.print(" ");
  Serial.print(analogRead(sound_toms_pin[3]));Serial.print(" ");
  Serial.print(analogRead(sound_snare_pin));Serial.print(" ");
  Serial.println(analogRead(sound_bass_pin));*/

  Serial.print("Pedal: ");Serial.print(digitalRead(pedal_pin));
  Serial.print(" Menu Switch: ");Serial.println(analogRead(menu_pin));
} 
