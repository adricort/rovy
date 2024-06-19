void Stroke_Function()
{
  digitalWrite(black_light_pin,HIGH);
  
  for(int i = 0; i < 4; i++)
  {
    analogWrite(toms_green[i],150);
    analogWrite(toms_red[i],255);
    analogWrite(toms_blue[i],0);
    sound_bass_value = analogRead(sound_bass_pin);

    // Bass Strokes:
    if(sound_bass_value < sound_reference)
    {
      digitalWrite(reflector_pin,HIGH);
      delay(30);
      digitalWrite(reflector_pin,LOW);
    }
    else
      digitalWrite(reflector_pin,LOW);
  }
  /*
  // Reading of colors
  G_value = analogRead(G_pin)/4;
  R_value = analogRead(R_pin)/4;
  B_value = analogRead(B_pin)/4;

  for(int i = 0; i < 4; i++)
  {
    // Sound Sensors Lecture (Toms, Snare, and Bass):
    sound_toms_value[i] = analogRead(sound_toms_pin[i]);
    sound_snare_value = analogRead(sound_snare_pin);
    sound_bass_value = analogRead(sound_bass_pin);

    // Menu Lecture always
    Menu_Lecture();
    // It considers fast live because it is used there
    if(menu_case != stroke_case && menu_case != fast_live_case)
    {
      smoke_counter_stroke = smoke_time_stroke;
      end_of_fast_live_flag = true;
      digitalWrite(smoke_machine_pin,LOW);
      goto BREAK;
    }
  }
    // Snare Stroke:
    if(sound_snare_value < sound_reference)
    {
      for(int j = 0; j < 4; j++)
      {
        analogWrite(toms_green[j],0);
        analogWrite(toms_red[j],255);
        analogWrite(toms_blue[j],0);
      }
      delay(5);
      for(int j = 0; j < 4; j++)
      {
        analogWrite(toms_green[j],0);
        analogWrite(toms_red[j],0);
        analogWrite(toms_blue[j],0);
      }
    }
    else
    {
      for(int j = 0; j < 4; j++)
      {
        analogWrite(toms_green[j],0);
        analogWrite(toms_red[j],0);
        analogWrite(toms_blue[j],0);
      }
    }

    // Toms Strokes:
    if(sound_toms_value[i] < sound_reference)
    {
      analogWrite(toms_green[i],G_value);
      analogWrite(toms_red[i],R_value);
      analogWrite(toms_blue[i],B_value);
      delay(5);
      analogWrite(toms_green[i],0);
      analogWrite(toms_red[i],0);
      analogWrite(toms_blue[i],0);
    }
    else
    {
      analogWrite(toms_green[i],0);
      analogWrite(toms_red[i],0);
      analogWrite(toms_blue[i],0);
    }

    // Bass Strokes:
    if(sound_bass_value < sound_reference)
    {
      digitalWrite(reflector_pin,HIGH);
      delay(30);
      digitalWrite(reflector_pin,LOW);
    }
    else
      digitalWrite(reflector_pin,LOW);
  }

  BREAK:
  {//Serial.println("STROKE BREAK");  // Don't uncomment this, it affects the smoke time
    }*/
}

void Stroke()
{
  // Pedal pin for smoke machine:
  if(digitalRead(pedal_pin) == true)
  {
    digitalWrite(smoke_machine_pin,HIGH);
    for(smoke_counter_stroke = 0; smoke_counter_stroke < smoke_time_stroke; smoke_counter_stroke++)
      Stroke_Function();
    digitalWrite(smoke_machine_pin,LOW);
  }
  else
  {
    digitalWrite(smoke_machine_pin,LOW);
    Stroke_Function();
  }
}
