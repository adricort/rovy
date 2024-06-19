void Blink_Green(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);

    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_green[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_green[i],LOW);
    Read_Pedal();
  }
}

void Blink_Blue(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);

    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_blue[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
  }
}

void Blink_Red(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_blue[i],LOW);

    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_red[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_red[i],LOW);
    Read_Pedal();
  }
}

void Blink_Cyan(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_red[i],LOW);

    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_green[i],HIGH);
      digitalWrite(toms_blue[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
  }
}

void Blink_Pink(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);

    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
  }
}

void Blink_Yellow(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_blue[i],LOW);

    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_green[i],HIGH);
      digitalWrite(toms_red[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);
    Read_Pedal();
  }
}

void Blink_White(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_green[i],HIGH);
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
  }
}

void Blink_Mix(int velocity)
{  
  if(velocity == 0)
    blink_time = 10000;
  else
    blink_time = 3000;
    
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    
    for(int h = 0; h < blink_time; h++)
    {
      digitalWrite(toms_mix[i],HIGH);
      Read_Pedal();
    }
    digitalWrite(toms_mix[i],LOW);
    Read_Pedal();
  }
}
