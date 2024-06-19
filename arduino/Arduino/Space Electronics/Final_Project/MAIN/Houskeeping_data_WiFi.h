void float_WiFi_send(float value) // Function that sends the data through WiFi (telnet) for floatating point
{
  char buff[10] = {' '};          // buffer creation for the value word
  int data_int = (int)value;      // compute the integer part of the float value
  float data_float = (abs(value) - abs(data_int))*100000; // the same as the fractional part, but floatating
  int data_fra = (int)data_float; // compute the fractional part of the float value
  sprintf(buff, "%.2f", value);   // this is useful for the "string" conversion with 2 decimals
  serverClients[0].write(buff);   // writes the buffer value
  serverClients[0].write(",");    // writes a comma to continue with the data frame
}

void int_WiFi_send(int value)     // Function that sends the data through WiFi (telnet) for integers
{
  char buff[10] = {' '};          // buffer creation for the value word
  sprintf(buff, "%d", value);     // this is useful for the "string" conversion without decimals
  serverClients[0].write(buff);   // writes the buffer value

  // Note: the coma is not included here since the last value is an integer, and a coma must be avoided in the data frame
}

void Houskeeping_data_WiFi(){     // Function that gets the rssi value and writes all the data into the server
  uint8_t i;      // creates the variable i for the n. of clients
  if (wifiMulti.run() == WL_CONNECTED) {    // if it is connected to the WiFi
    rssi = WiFi.RSSI();                     // get the rssi value (quality of signal in decibels [dB])
    // If the Timer Interrupt has been triggered before, then send the data
    if(timer_flag == true)
    {
      serverClients[0].write("{");    // starts the string of the data frame to the client
      float_WiFi_send(gyrox);         // writes the gyrox data to the client
      float_WiFi_send(gyroy);         // writes the gyroy data to the client
      float_WiFi_send(gyroz);         // writes the gyroz data to the client
      float_WiFi_send(accelx);        // writes the accelx data to the client
      float_WiFi_send(accely);        // writes the accely data to the client
      float_WiFi_send(accelz);        // writes the accelz data to the client
      int_WiFi_send(magx);serverClients[0].write(",");        // writes the magx data and a coma to the client
      int_WiFi_send(magy);serverClients[0].write(",");        // writes the magx data and a coma to the client
      int_WiFi_send(magz);serverClients[0].write(",");        // writes the magx data and a coma to the client
      int_WiFi_send(sang);serverClients[0].write(",");        // writes the servo angle data and a coma to the client
      float_WiFi_send(temp);        // writes the temperature data to the client
      float_WiFi_send(hum);         // writes the humidity data to the client
      float_WiFi_send(pres);        // writes the pressure data to the client
      float_WiFi_send(rssi);        // writes the rssi data to the client
      int_WiFi_send(ldr);           // writes the light intensity data to the client
      serverClients[0].write("}");  // finishes the string of the frame to the client
      serverClients[0].write("\n"); // enters a tab command for better data printing
      
      // Printing the data into the serial monitor just to verify that it matches with the WiFi client:
      /*Serial.print("Gx,y,z: ");Serial.print(gyrox);Serial.print(",");Serial.print(gyroy);Serial.print(",");Serial.print(gyroz);
      Serial.print(" Ax,y,z: ");Serial.print(accelx);Serial.print(",");Serial.print(accely);Serial.print(",");Serial.print(accelz);
      Serial.print(" Mx,y,z: ");Serial.print(magx);Serial.print(",");Serial.print(magy);Serial.print(",");Serial.print(magz);
      Serial.print(" Servo: ");Serial.print(sang);
      Serial.print(" Temp: ");Serial.print(temp);
      Serial.print(" Hum: ");Serial.print(hum);
      Serial.print(" Pres: ");Serial.print(pres);
      Serial.print(" RSSI: ");Serial.print(rssi);
      Serial.print(" LDR: ");Serial.print(ldr);
      Serial.println();*/
      
      timer_flag = false;   // restarts the timer interrupt flag to wait for the next interruption
    }

    // If the connection with the client is broken
    if (server.hasClient()){    // First, it verifies that there is a client
      for(i = 0; i < MAX_SRV_CLIENTS; i++){ // Until it reaches the maximum number of clients
        // find free/disconnected spot
        if (!serverClients[i] || !serverClients[i].connected()){
          if(serverClients[i]) serverClients[i].stop();
          serverClients[i] = server.available();
          if (!serverClients[i]) Serial.println("available broken");
          Serial.print("New client: ");
          Serial.print(i); Serial.print(' ');
          Serial.println(serverClients[i].remoteIP());
          break;
        }
      }
      if (i >= MAX_SRV_CLIENTS) {
        //no free/disconnected spot so reject
        server.available().stop();
      }
    }
    //check clients for data
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      // If the server clients are connected
      if (serverClients[i] && serverClients[i].connected()){
        if(serverClients[i].available()){
          //get data from the telnet client and push it to the UART
          while(serverClients[i].available())
          {
            Mode = serverClients[i].parseInt();
            display.clearDisplay();     // clear the display
            display.display();          // update the display
            // Delimitation of modes
            if(Mode > 5)
              Mode = 5;
            if(Mode < 0)
              Mode = 0;
            for(int i = 0; i < Mode; i++)
            {
              ledcWriteTone(5, 1000); // turns on the buzzer with channel 5 at 500 Hz
              delay(50);              // wait 50 miliseconds
              ledcWriteTone(5, 0);    // turns off the buzzer
              delay(50);              // wait 50 miliseconds
            }
          }  
        }
      }
      else {  // but if they are not connected
        if (serverClients[i]) {
          serverClients[i].stop();    // it stops the client
        }
      }
    }
  }
  else {    // but if it is not connected to the WiFi 
    Serial.println("WiFi not connected!");  // prints the message in the serial monitor
    for(i = 0; i < MAX_SRV_CLIENTS; i++) {    // stops all the clients
      if (serverClients[i]) serverClients[i].stop();
    }
    delay(1000);      // waits for 1 second
  }
}
