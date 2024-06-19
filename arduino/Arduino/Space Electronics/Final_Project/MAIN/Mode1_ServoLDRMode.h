// Function that moves the servo depending on the light intensity of the LDR
void Mode1_ServoLDRMode(int ldr, int CHANNEL){
  display.setCursor(0,0);                 // goes to the beginning of the matrix
  display.print("Mode: 1 ServoLDRMode");  // displays the mode on the matrix
  display.display();                      // updates the buffer
  ledcWriteTone(CHANNEL, 0);              // turns off the buzzer
  digitalWrite(LEDgreen_pin,LOW);         // turns off the green LED
  digitalWrite(LEDyellow_pin,LOW);        // turns off the yellow LED
  digitalWrite(LEDred_pin,LOW);           // turns off the red LED
  
  int angle = int(ldr*180/4096);          // Proportional controller for the servo based on the light intensity of the ldr
  myservo.write(angle);                   // writes the servo angle
  
  // reads the touch buttons on each interation
  TouchRead(CHANNEL);  // this actually has a better performance than the touchInterrupt
}
