void Serial_Configuration()
{
  if(Serial.available() > 0)    // Checks whether data is comming from the serial port
  {
    communication_stablished_flag = true;   
    String configuration = Serial.readStringUntil(36);  // 36 = $

    int indice_0 = configuration.lastIndexOf('p');          // permanents (0-9) and gradients (10-11)
    int indice_1 = configuration.indexOf('s',indice_0+1);   // strokes
    int indice_2 = configuration.indexOf('r',indice_1+1);   // reflector
    int indice_3 = configuration.indexOf('k',indice_2+1);   // smoke machine
    int indice_4 = configuration.indexOf('a',indice_3+1);

    String number_1 = configuration.substring(indice_0+1,indice_1);
    String number_2 = configuration.substring(indice_1+1,indice_2);
    String number_3 = configuration.substring(indice_2+1,indice_3);
    String number_4 = configuration.substring(indice_3+1,indice_4);

    int permanent_code = number_1.toInt();
    int stroke_code = number_2.toInt();
    int reflector_code = number_3.toInt();
    int smoke_code = number_4.toInt();

    // Example: p1s0r0k0a$

    Serial.print("SERIAL:PERMANENT=");Serial.print(permanent_code);

    switch(permanent_code)
    {
      case 0:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],LOW);
          digitalWrite(toms_red[i],LOW);
          digitalWrite(toms_blue[i],LOW);
        }
        break;
      case 1:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          digitalWrite(toms_red[i],LOW);
          digitalWrite(toms_blue[i],LOW);
        }
        break;
      case 2:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],LOW);
          digitalWrite(toms_red[i],HIGH);
          digitalWrite(toms_blue[i],LOW);
        }
        break;
      case 3:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],LOW);
          digitalWrite(toms_red[i],LOW);
          digitalWrite(toms_blue[i],HIGH);
        }
        break;
      case 4:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          digitalWrite(toms_red[i],LOW);
          digitalWrite(toms_blue[i],HIGH);
        }
        break;
      case 5:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          digitalWrite(toms_red[i],HIGH);
          digitalWrite(toms_blue[i],LOW);
        }
        break;
      case 6:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],LOW);
          digitalWrite(toms_red[i],HIGH);
          digitalWrite(toms_blue[i],HIGH);
        }
        break;
      case 7:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],HIGH);
          digitalWrite(toms_red[i],HIGH);
          digitalWrite(toms_blue[i],HIGH);
        }
        break;
      case 8:
        digitalWrite(black_light_pin,LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],LOW);
          digitalWrite(toms_red[i],LOW);
          digitalWrite(toms_blue[i],LOW);
        }
        for(int i = 0; i < 6; i++)
          digitalWrite(toms_mix[i],HIGH);
        break;
      case 9:
        digitalWrite(black_light_pin,HIGH);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(toms_green[i],0);
          digitalWrite(toms_red[i],0);
          digitalWrite(toms_blue[i],0);
        }
        break;
      case 10:
        digitalWrite(black_light_pin,LOW);
        while(Serial.available() == 0)
          Gradient();
        break;
      default:
        break;
    }

    switch(reflector_code)
    {
      case 0:
        digitalWrite(reflector_pin,LOW);
        break;
      case 1:
        digitalWrite(reflector_pin,HIGH);
        break;
      default:
        break;
    }

    switch(smoke_code)
    {
      case 0:
        digitalWrite(smoke_machine_pin,LOW);
        break;
      case 1:
        digitalWrite(smoke_machine_pin,HIGH);
        delay(7000);
        digitalWrite(smoke_machine_pin,LOW);
        smoke_code = 0;   // Para evitar que vaya a activarse infinitvamente, prevencion solamente
        break;
      default:
        break;
    }

    while(Serial.available() == 0)
    {
      switch(stroke_code)
      {
        case 1:
          // Lectura de sensores de sonido
          for(int i = 0; i < 4; i++)
          {
            // Sound Sensors Lecture (Toms, Snare, and Bass):
            sound_toms_value[i] = analogRead(sound_toms_pin[i]);
            sound_snare_value = analogRead(sound_snare_pin);
            sound_bass_value = analogRead(sound_bass_pin);
  
            // Snare Stroke:
            if(sound_snare_value < sound_reference)
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],HIGH);
              }
              delay(3);
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
              }
            }
            else
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
        
            // Toms Strokes:
            if(sound_toms_value[i] < sound_reference)
            {
              digitalWrite(toms_green[i],HIGH);
              delay(5);
              digitalWrite(toms_green[i],LOW);
            }
            else
            {
              digitalWrite(toms_green[i],LOW);
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
          break;
        case 2:
          // Lectura de sensores de sonido
          for(int i = 0; i < 4; i++)
          {
            // Sound Sensors Lecture (Toms, Snare, and Bass):
            sound_toms_value[i] = analogRead(sound_toms_pin[i]);
            sound_snare_value = analogRead(sound_snare_pin);
            sound_bass_value = analogRead(sound_bass_pin);
  
            // Snare Stroke:
            if(sound_snare_value < sound_reference)
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_red[j],HIGH);
              }
              delay(3);
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_red[j],LOW);
              }
            }
            else
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
        
            // Toms Strokes:
            if(sound_toms_value[i] < sound_reference)
            {
              digitalWrite(toms_red[i],HIGH);
              delay(5);
              digitalWrite(toms_red[i],LOW);
            }
            else
            {
              digitalWrite(toms_red[i],LOW);
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
          break;
        case 3:
          // Lectura de sensores de sonido
          for(int i = 0; i < 4; i++)
          {
            // Sound Sensors Lecture (Toms, Snare, and Bass):
            sound_toms_value[i] = analogRead(sound_toms_pin[i]);
            sound_snare_value = analogRead(sound_snare_pin);
            sound_bass_value = analogRead(sound_bass_pin);
            
            // Snare Stroke:
            if(sound_snare_value < sound_reference)
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_blue[j],HIGH);
              }
              delay(3);
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_blue[j],LOW);
              }
            }
            else
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
        
            // Toms Strokes:
            if(sound_toms_value[i] < sound_reference)
            {
              digitalWrite(toms_blue[i],HIGH);
              delay(5);
              digitalWrite(toms_blue[i],LOW);
            }
            else
            {
              digitalWrite(toms_red[i],LOW);
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
          break;
        case 4:
          // Lectura de sensores de sonido
          for(int i = 0; i < 4; i++)
          {
            // Sound Sensors Lecture (Toms, Snare, and Bass):
            sound_toms_value[i] = analogRead(sound_toms_pin[i]);
            sound_snare_value = analogRead(sound_snare_pin);
            sound_bass_value = analogRead(sound_bass_pin);
            
            // Snare Stroke:
            if(sound_snare_value < sound_reference)
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],HIGH);
                digitalWrite(toms_blue[j],HIGH);
              }
              delay(3);
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
            else
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
        
            // Toms Strokes:
            if(sound_toms_value[i] < sound_reference)
            {
              digitalWrite(toms_green[i],HIGH);
              digitalWrite(toms_blue[i],HIGH);
              delay(5);
              digitalWrite(toms_green[i],LOW);
              digitalWrite(toms_blue[i],LOW);
            }
            else
            {
              digitalWrite(toms_green[i],LOW);
              digitalWrite(toms_blue[i],LOW);
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
          break;
        case 5:
          // Lectura de sensores de sonido
          for(int i = 0; i < 4; i++)
          {
            // Sound Sensors Lecture (Toms, Snare, and Bass):
            sound_toms_value[i] = analogRead(sound_toms_pin[i]);
            sound_snare_value = analogRead(sound_snare_pin);
            sound_bass_value = analogRead(sound_bass_pin);
            
            // Snare Stroke:
            if(sound_snare_value < sound_reference)
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],HIGH);
                digitalWrite(toms_red[j],HIGH);
              }
              delay(3);
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
              }
            }
            else
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
        
            // Toms Strokes:
            if(sound_toms_value[i] < sound_reference)
            {
              digitalWrite(toms_green[i],HIGH);
              digitalWrite(toms_red[i],HIGH);
              delay(5);
              digitalWrite(toms_green[i],LOW);
              digitalWrite(toms_red[i],LOW);
            }
            else
            {
              digitalWrite(toms_green[i],LOW);
              digitalWrite(toms_red[i],LOW);
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
          break;
        case 6:
          // Lectura de sensores de sonido
          for(int i = 0; i < 4; i++)
          {
            // Sound Sensors Lecture (Toms, Snare, and Bass):
            sound_toms_value[i] = analogRead(sound_toms_pin[i]);
            sound_snare_value = analogRead(sound_snare_pin);
            sound_bass_value = analogRead(sound_bass_pin);
            
            // Snare Stroke:
            if(sound_snare_value < sound_reference)
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_blue[j],HIGH);
                digitalWrite(toms_red[j],HIGH);
              }
              delay(3);
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_blue[j],LOW);
                digitalWrite(toms_red[j],LOW);
              }
            }
            else
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
        
            // Toms Strokes:
            if(sound_toms_value[i] < sound_reference)
            {
              digitalWrite(toms_blue[i],HIGH);
              digitalWrite(toms_red[i],HIGH);
              delay(5);
              digitalWrite(toms_blue[i],LOW);
              digitalWrite(toms_red[i],LOW);
            }
            else
            {
              digitalWrite(toms_blue[i],LOW);
              digitalWrite(toms_red[i],LOW);
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
          break;
        case 7:
          // Lectura de sensores de sonido
          for(int i = 0; i < 4; i++)
          {
            // Sound Sensors Lecture (Toms, Snare, and Bass):
            sound_toms_value[i] = analogRead(sound_toms_pin[i]);
            sound_snare_value = analogRead(sound_snare_pin);
            sound_bass_value = analogRead(sound_bass_pin);
            
            // Snare Stroke:
            if(sound_snare_value < sound_reference)
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],HIGH);
                digitalWrite(toms_red[j],HIGH);
                digitalWrite(toms_blue[j],HIGH);
              }
              delay(3);
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
            else
            {
              for(int j = 0; j < 4; j++)
              {
                digitalWrite(toms_green[j],LOW);
                digitalWrite(toms_red[j],LOW);
                digitalWrite(toms_blue[j],LOW);
              }
            }
        
            // Toms Strokes:
            if(sound_toms_value[i] < sound_reference)
            {
              digitalWrite(toms_green[i],HIGH);
              digitalWrite(toms_red[i],HIGH);
              digitalWrite(toms_blue[i],HIGH);
              delay(5);
              digitalWrite(toms_green[i],LOW);
              digitalWrite(toms_red[i],LOW);
              digitalWrite(toms_blue[i],LOW);
            }
            else
            {
              digitalWrite(toms_green[i],LOW);
              digitalWrite(toms_red[i],LOW);
              digitalWrite(toms_blue[i],LOW);
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
          break;
      }
    }
  }
  else if(communication_stablished_flag == false)
  {
    for(int i = 0; i < 4; i++)
    {
      analogWrite(toms_green[i],0);
      analogWrite(toms_red[i],0);
      analogWrite(toms_blue[i],0);
    }
  }
}
