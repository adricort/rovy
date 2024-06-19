// Function that rings a specific tone of the buzzer for a period of time
void note(int CHANNEL, float frequency, int milisecs)
{
  int timee;                    // this helps for the milisecond counting (delay simulator)
  int time_previous = millis(); // the previous value of the simulator initialization
  if(Mode == 2)
    ledcWriteTone(CHANNEL, frequency);  // rings the specified buzzer tone
  while(1)    // enters an infinite loop until the time or any other external interruption rises
  {
    timee = millis() - time_previous;   // updating the value
    // Note for the next function: the 0.09 factor was obtained by measuring the time used for the magnetometer
    // In order to avoid this time loss, we have to decrease the duration to match the time perfectly to 3 seconds
    if(timee > (milisecs-int(milisecs*0.11))) // If the specified time for the note is exceeded
      break;    // it breaks the infinite loop

    if (Mode == 2)    // This is to ensure that we are still in Mode 2
    {
      TouchRead(CHANNEL);              // reads the touch buttons on each interation
      LSM6DS33_getvalues(gyroz, gyroy, gyroz, accelx, accely, accelz);  // Sensor values of the IMU
      MLX90393_getvalues(magx, magy, magz);                     // Sensor values of the magnetometer
      Servo_getvalues(sang);                                    // Encoder values of the Servo
      BME280_getvalues(temp, hum, pres);                        // Sensor values of the humidity sensor
      LDR_getvalues(ldr);                                       // Sensor values of the LDR
      Houskeeping_data_WiFi();                                  // Sensor values of the rssi and Houskeeping-data sending
    }
    else    // but if it is not in Mode 2 anymore
    {
      display.clearDisplay();     // clears the buffer
      display.display();          // display the buffer
      ledcWriteTone(CHANNEL, 0);  // turns off the buzzer
      break;                      // breaks the infinite loop
    }
   
    if(serverClients[0].available())    // if it receives commands from server terminal
    {
      ledcWriteTone(CHANNEL, 0);    // turns off the buzzer
      Mode = serverClients[0].parseInt(); // gets the integer part of the command
      for(int i = 0; i < Mode; i++) // Ring the buzzer depending on the mode
      {
        ledcWriteTone(CHANNEL, 1000);   // turns on the buzzer with channel at 1000 Hz
        delay(50);                      // waits 50 miliseconds
        ledcWriteTone(CHANNEL, 0);      // turns off the buzzer
        delay(50);                      // waits 50 miliseconds
      }
    }      
  }
}

// Function that rings a beacon melody within 3 seconds, every 10 seconds
void Mode2_MelodyMode(int CHANNEL){
  display.setCursor(0,0);               // goes to the beginning of the matrix
  display.print("Mode: 2 MelodyMode");  // displays the mode on the matrix
  display.display();                    // updates the buffer
  digitalWrite(LEDgreen_pin,LOW);       // turns off the green LED
  digitalWrite(LEDyellow_pin,LOW);      // turns off the yellow LED
  digitalWrite(LEDred_pin,LOW);         // turns off the red LED

  // La Cucaracha song :D (mexican)
  // In total, the miliseconds are = 3000 = 3 secs (channel,frequency in Hz,duration in ms)
  note(CHANNEL,196,125);  // do       LA
  note(CHANNEL,196,125);  // do       CU
  note(CHANNEL,196,125);  // do       CA
  note(CHANNEL,262,250);  // fa       RA
  note(CHANNEL,329,250);  // la       CHA
  note(CHANNEL,196,125);  // do       LA
  note(CHANNEL,196,125);  // do       CU
  note(CHANNEL,196,125);  // do       CA
  note(CHANNEL,262,250);  // fa       RA
  note(CHANNEL,329,500);  // la       CHA
  note(CHANNEL,262,250);  // fa       YA
  note(CHANNEL,262,125);  // fa       NO
  note(CHANNEL,247,125);  // mi       PUE
  note(CHANNEL,247,125);  // mi       DE
  note(CHANNEL,220,125);  // re       CA
  note(CHANNEL,220,125);  // re       MI
  note(CHANNEL,196,125);  // do       NAR

  int timee;                    // this helps for the milisecond counting (delay simulator)
  int time_previous = millis(); // the previous value of the simulator initialization
  if(Mode == 2)
    ledcWriteTone(CHANNEL, 0);  // rings the specified buzzer tone
  while(1)    // enters an infinite loop until the time or any other external interruption rises
  {
    timee = millis() - time_previous;   // updating the value

    // Note for the next conditional:
    // it was measured that there is a loss of 10 miliseconds due to magnetometer readings
    if(timee > 10000-15)    // If the specified time for the note is exceeded
      break;                // breaks the infinite loop
      
    if (Mode == 2)    // This is to ensure that we are still in Mode 2
    {
      TouchRead(CHANNEL);              // reads the touch buttons on each interation
      LSM6DS33_getvalues(gyroz, gyroy, gyroz, accelx, accely, accelz);  // Sensor values of the IMU
      MLX90393_getvalues(magx, magy, magz);                     // Sensor values of the magnetometer
      Servo_getvalues(sang);                                    // Encoder values of the Servo
      BME280_getvalues(temp, hum, pres);                        // Sensor values of the humidity sensor
      LDR_getvalues(ldr);                                       // Sensor values of the LDR
      Houskeeping_data_WiFi();                                  // Sensor values of the rssi and Houskeeping-data sending
    }
    else    // but if it is not in Mode 2 anymore
    {
      display.clearDisplay();     // clears the buffer
      display.display();          // display the buffer
      break;                      // breaks the infinite loop
    }
    
    if(serverClients[0].available())  // if it receives commands from server terminal
    {
      ledcWriteTone(CHANNEL, 0);            // Turns off the buzzer
      Mode = serverClients[0].parseInt();   // gets the integer part of the command
      for(int i = 0; i < Mode; i++) // Ring the buzzer depending on the mode
      {
        ledcWriteTone(CHANNEL, 1000);   // turns on the buzzer with channel at 1000 Hz
        delay(50);                      // waits 50 miliseconds
        ledcWriteTone(CHANNEL, 0);      // turns off the buzzer
        delay(50);                      // waits 50 miliseconds
      }
    }
  }
}
