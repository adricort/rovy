void Permanent()
{
  first_gradient_flag = false;
  G_value = analogRead(G_pin)/4;
  R_value = analogRead(R_pin)/4;
  B_value = analogRead(B_pin)/4;
  
  for(int i = 0; i < 4; i++)
  {
    analogWrite(toms_green[i],G_value);
    analogWrite(toms_red[i],R_value);
    analogWrite(toms_blue[i],B_value);
  }

  // Pedal pin for smoke machine:
  Smoke_Machine(permanent_case);
}
