void Permanent_Green()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],HIGH);
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
  }
}

void Permanent_Red()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],HIGH);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
  }
}

void Permanent_Blue()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],HIGH);
    Read_Pedal();
  }
}

void Permanent_Cyan()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_green[i],HIGH);
    digitalWrite(toms_blue[i],HIGH);
    Read_Pedal();
  }
}

void Permanent_Pink()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],HIGH);
    digitalWrite(toms_blue[i],HIGH);
    Read_Pedal();
  }
}

void Permanent_Yellow()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],HIGH);
    digitalWrite(toms_red[i],HIGH);
    digitalWrite(toms_blue[i],LOW);
    Read_Pedal();
  }
}

void Permanent_White()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],HIGH);
    digitalWrite(toms_red[i],HIGH);
    digitalWrite(toms_blue[i],HIGH);
    Read_Pedal();
  }
}

void Permanent_Mix()
{
  for(int i = 0; i < 6; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);
    digitalWrite(toms_mix[i],HIGH);
    Read_Pedal();
  }
}

void Permanent_Black()
{
  digitalWrite(black_pin,HIGH);
  //Read_Pedal();
}

void Permanent_Reflector()
{
  detachInterrupt(digitalPinToInterrupt(sensor_bass));
  digitalWrite(reflector_pin,HIGH);
  Read_Pedal();
}

