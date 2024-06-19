void Gradient()
{  
  if(first_gradient_flag == false)
  {
    // White
    for(gradient_counter = 0; gradient_counter < 255; gradient_counter++)
    {
      for(int i = 0; i < 4; i++)
      {
        Serial.println("Color: White");
        analogWrite(toms_green[i],gradient_counter);
        analogWrite(toms_red[i],gradient_counter);
        analogWrite(toms_blue[i],gradient_counter);

        // Menu Lecture always
        Menu_Lecture();
        // It has serial case so that you can change during the gradient
        if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
          goto BREAK;
      }
    }
  }
  else
  {
    // White
    for(gradient_counter = 0; gradient_counter < 255; gradient_counter++)
    {
      for(int i = 0; i < 4; i++)
      {
        Serial.println("Color: White");
        analogWrite(toms_green[i],gradient_counter);
        analogWrite(toms_red[i],gradient_counter);
        Menu_Lecture();
        if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
          goto BREAK;
      }
    }
  }

  // Cyan
  for(gradient_counter = 255; gradient_counter > 0; gradient_counter--)
  {
    for(int i = 0; i < 4; i++)
    {
      Serial.println("Color: Cyan");
      analogWrite(toms_red[i],gradient_counter);
      Menu_Lecture();
      if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
        goto BREAK;
    }
  }
  
  // Green
  for(gradient_counter = 255; gradient_counter > 0; gradient_counter--)
  {
    for(int i = 0; i < 4; i++)
    {
      Serial.println("Color: Green");
      analogWrite(toms_blue[i],gradient_counter);
      Menu_Lecture();
      if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
        goto BREAK;
    }
  }

  // Yellow
  for(gradient_counter = 0; gradient_counter < 255; gradient_counter++)
  {
    for(int i = 0; i < 4; i++)
    {
      Serial.println("Color: Yellow");
      analogWrite(toms_red[i],gradient_counter);
      Menu_Lecture();
      if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
        goto BREAK;
    }
  }

  // Red
  for(gradient_counter = 255; gradient_counter > 0; gradient_counter--)
  {
    for(int i = 0; i < 4; i++)
    {
      Serial.println("Color: Red");
      analogWrite(toms_green[i],gradient_counter);
      Menu_Lecture();
      if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
        goto BREAK;
    }
  }

  // Pink
  for(gradient_counter = 0; gradient_counter < 255; gradient_counter++)
  {
    for(int i = 0; i < 4; i++)
    {
      Serial.println("Color: Pink");
      analogWrite(toms_blue[i],gradient_counter);
      Menu_Lecture();
      if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
        goto BREAK;
    }
  }

  // Blue
  for(gradient_counter = 255; gradient_counter > 0; gradient_counter--)
  {
    for(int i = 0; i < 4; i++)
    {
      Serial.println("Color: Blue");
      analogWrite(toms_red[i],gradient_counter);
      Menu_Lecture();
      if((menu_case != gradient_case && menu_case != serial_configuration_case) || (Serial.available() > 0))
        goto BREAK;
    }
  }
  
  first_gradient_flag = true;

  BREAK:
  {Serial.println("GRADIENT BROKEN");}
}
