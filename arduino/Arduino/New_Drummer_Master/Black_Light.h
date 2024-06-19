void Black_Light()
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(toms_green[i],LOW);
    digitalWrite(toms_red[i],LOW);
    digitalWrite(toms_blue[i],LOW);
  }
  
  digitalWrite(black_light_pin,HIGH);

  // Pedal pin for smoke machine:
  Smoke_Machine(black_light_case);

  // To turn off black light before going to another state
  Menu_Lecture();
  if(menu_case != black_light_case)
    digitalWrite(black_light_pin,LOW);
}
