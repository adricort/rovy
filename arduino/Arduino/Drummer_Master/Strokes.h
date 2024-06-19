void Stroke_Green()
{
  if(analogRead(sensor_snare) < 50)
  {
    Serial.println(analogRead(sensor_snare));
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],LOW);
      digitalWrite(toms_blue[i],LOW);
      digitalWrite(toms_green[i],HIGH);
      delay(15);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
      digitalWrite(toms_green[i],LOW);
  }
  
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
    if(analogRead(sensor_toms[i]) < 50)
    {
      digitalWrite(toms_green[i],HIGH);
      delay(15);
    }
    else
      digitalWrite(toms_green[i],LOW);
  }
}

void Stroke_Red()
{
  if(analogRead(sensor_snare) < 50)
  {
    Serial.println(analogRead(sensor_snare));
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],LOW);
      digitalWrite(toms_green[i],LOW);
      delay(15);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
      digitalWrite(toms_red[i],LOW);
  }
  
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
    if(analogRead(sensor_toms[i]) < 50)
    {
      digitalWrite(toms_red[i],HIGH);
      delay(15);
    }
    else
      digitalWrite(toms_red[i],LOW);
  }
}

void Stroke_Blue()
{
  if(analogRead(sensor_snare) < 50)
  {
    Serial.println(analogRead(sensor_snare));
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],LOW);
      digitalWrite(toms_blue[i],HIGH);
      digitalWrite(toms_green[i],LOW);
      delay(15);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
      digitalWrite(toms_blue[i],LOW);
  }
  
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);
    Read_Pedal();
    if(analogRead(sensor_toms[i]) < 50)
    {
      digitalWrite(toms_blue[i],HIGH);
      delay(15);
    }
    else
      digitalWrite(toms_blue[i],LOW);
  }
}

void Stroke_Cyan()
{
  if(analogRead(sensor_snare) < 50)
  {
    Serial.println(analogRead(sensor_snare));
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],LOW);
      digitalWrite(toms_blue[i],HIGH);
      digitalWrite(toms_green[i],HIGH);
      delay(15);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_green[i],LOW);
      digitalWrite(toms_blue[i],LOW);
    }
  }
  
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_red[i],LOW);
    Read_Pedal();
    if(analogRead(sensor_toms[i]) < 50)
    {
      digitalWrite(toms_blue[i],HIGH);
      digitalWrite(toms_green[i],HIGH);
      delay(15);
    }
    else
    {
      digitalWrite(toms_blue[i],LOW);
      digitalWrite(toms_green[i],LOW);
    }
  }
}

void Stroke_Pink()
{
  if(analogRead(sensor_snare) < 50)
  {
    Serial.println(analogRead(sensor_snare));
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],HIGH);
      digitalWrite(toms_green[i],LOW);
      delay(15);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],LOW);
      digitalWrite(toms_blue[i],LOW);
    }
  }
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    Read_Pedal();
    if(analogRead(sensor_toms[i]) < 50)
    {
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],HIGH);
      delay(15);
    }
    else
    {
      digitalWrite(toms_red[i],LOW);
      digitalWrite(toms_blue[i],LOW);
    }
  }
}

void Stroke_Yellow()
{
  if(analogRead(sensor_snare) < 50)
  {
    Serial.println(analogRead(sensor_snare));
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],LOW);
      digitalWrite(toms_green[i],HIGH);
      delay(15);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_green[i],LOW);
      digitalWrite(toms_red[i],LOW);
    }
  }
  
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
    if(analogRead(sensor_toms[i]) < 50)
    {
      digitalWrite(toms_green[i],HIGH);
      digitalWrite(toms_red[i],HIGH);
      delay(20);
    }
    else
    {
      digitalWrite(toms_green[i],LOW);
      digitalWrite(toms_red[i],LOW);
    }
  }
}

void Stroke_White()
{
  if(analogRead(sensor_snare) < 50)
  {
    Serial.println(analogRead(sensor_snare));
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],HIGH);
      digitalWrite(toms_green[i],HIGH);
      delay(15);
    }
  }
  else
  {
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(toms_red[i],LOW);
      digitalWrite(toms_green[i],LOW);
      digitalWrite(toms_blue[i],LOW);
    }
  }
  
  for(int i = 0; i < 4; i++)
  {
    Read_Pedal();
    if(analogRead(sensor_toms[i]) < 50)
    {
      digitalWrite(toms_green[i],HIGH);
      digitalWrite(toms_red[i],HIGH);
      digitalWrite(toms_blue[i],HIGH);
      delay(20);
    }
    else
    {
      digitalWrite(toms_green[i],LOW);
      digitalWrite(toms_red[i],LOW);
      digitalWrite(toms_blue[i],LOW);
    }
  }
}

void Stroke_Bass()
{
  for(int delayer_bass = 0; delayer_bass < 2000; delayer_bass++)
    digitalWrite(reflector_pin, HIGH);
  digitalWrite(reflector_pin,LOW);
}


