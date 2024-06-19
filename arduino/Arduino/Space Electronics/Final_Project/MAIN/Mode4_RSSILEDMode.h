// Function that turns on a LED that has a color-dependient of the WiFi rssi signal
void Mode4_RSSILEDMode(float rssi, int CHANNEL){
  display.setCursor(0,0);               // goes to the beginning of the matrix
  display.print("Mode: 4 RSSILEDMode"); // displays the mode on the matrix
  display.display();                    // updates the buffer
  ledcWriteTone(CHANNEL, 0);            // turns off the buzzer

  if(rssi >= -33) // if the signal is higher or equal to the specified value
  {
    digitalWrite(LEDgreen_pin,HIGH);         // turns on the green LED
    digitalWrite(LEDyellow_pin,LOW);        // turns off the yellow LED
    digitalWrite(LEDred_pin,LOW);           // turns off the red LED
  }
  else if(rssi >= -45)  // if the signal is higher or equal to the specified value
  {
    digitalWrite(LEDgreen_pin,LOW);         // turns off the green LED
    digitalWrite(LEDyellow_pin,HIGH);       // turns on the yellow LED
    digitalWrite(LEDred_pin,LOW);           // turns off the red LED
  }
  else  // if the signal is any other case
  {
    digitalWrite(LEDgreen_pin,LOW);         // turns off the green LED
    digitalWrite(LEDyellow_pin,LOW);        // turns off the yellow LED
    digitalWrite(LEDred_pin,HIGH);          // turns on the red LED
  }

  TouchRead(CHANNEL);       // reads the touch buttons on each interation
}
