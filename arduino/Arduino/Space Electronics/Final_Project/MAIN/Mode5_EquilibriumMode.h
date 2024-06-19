// Function that controlls the servo depending on the inclination of the board in the x axis of the accelerometer (+LEDs)
void Mode5_EquilibriumMode(int CHANNEL, float accelx){
  display.setCursor(0,0);       // goes to the beginning of the matrix
  display.print("Mode: 5 EquilibriumMode");   // displays the mode on the matrix
  display.display();            // updates the buffer

  float frequency_inclination = 3000*abs(accelx);   // P-conroller for the frequency from 0 to 3000 depending on the accelx
  ledcWriteTone(CHANNEL, frequency_inclination);    // write to the buzzer the computed freq. of inclination

  int servo_inclination_angle = 90;     // initialization of angle
  if(accelx >= 0)   // If it goes to the right
    servo_inclination_angle = int((90*accelx+90)*1.2);    // P-controller*oversized factor
  else      // If it goes to the left
    servo_inclination_angle = int((90-90*abs(accelx))*1.2); // P-controller*oversized factor

  // Angle range delimiation
  if (servo_inclination_angle > 180)
    servo_inclination_angle = 180;
  else if(servo_inclination_angle < 0)
    servo_inclination_angle = 0;

  // Similar to the RSSILEDMode (4), it turns on the LEDs depending on some range [g]
  if (abs(accelx) >= 0 && abs(accelx) < 0.33)
  {
    digitalWrite(LEDgreen_pin,HIGH);
    digitalWrite(LEDyellow_pin,LOW);
    digitalWrite(LEDred_pin,LOW);
  }
  else if (abs(accelx) >= 0.33 && abs(accelx) < 0.66)
  {
    digitalWrite(LEDgreen_pin,LOW);
    digitalWrite(LEDyellow_pin,HIGH);
    digitalWrite(LEDred_pin,LOW);
  }
  else if (abs(accelx) >= 0.66)
  {
    digitalWrite(LEDgreen_pin,LOW);
    digitalWrite(LEDyellow_pin,LOW);
    digitalWrite(LEDred_pin,HIGH);
  }

  TouchRead(CHANNEL);       // reads the touch buttons on each interation
  myservo.write(servo_inclination_angle);   // write the computed angle to the servo
}
