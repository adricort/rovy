void Turn_Off()
{
  for(int i = 0; i < 12; i++)
    digitalWrite(toms[i],LOW);
}

void Turn_Off_Reflector()
{
  detachInterrupt(digitalPinToInterrupt(sensor_bass));
  digitalWrite(reflector_pin,LOW);
}

void Read_Pedal()
{
  pedal_value = digitalRead(pedal_pin);
  if(pedal_value != pedal_last_value)
    if(pedal_value == true)
    {
      pedal_flag = !pedal_flag;
      delay(50);
    }
  pedal_last_value = pedal_value;
}

