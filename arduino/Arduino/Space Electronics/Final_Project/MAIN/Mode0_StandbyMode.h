// Function that waits for commands or touch buttons
void Mode0_StandbyMode(int CHANNEL){
  display.setCursor(0,0);               // goes to the beginning of the matrix
  display.print("Mode: 0 StandbyMode"); // displays the mode on the matrix
  display.display();                    // updates the buffer
  ledcWriteTone(CHANNEL, 0);            // turns off the buzzer
  digitalWrite(LEDgreen_pin,LOW);       // turns off the green LED
  digitalWrite(LEDyellow_pin,LOW);      // turns off the yellow LED
  digitalWrite(LEDred_pin,LOW);         // turns off the red LED

  // reads the touch buttons on each interation
  TouchRead(CHANNEL);  // this actually has a better performance than the touchInterrupt
}
