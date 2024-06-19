// Function that makes the servo point to the magnetic north based on the magnetometer measurements
void Mode3_CompassMode(int magx, int magy, int magz, int CHANNEL){
  display.setCursor(0,0);                 // goes to the beginning of the matrix
  display.print("Mode: 3 CompassMode");   // displays the mode on the matrix
  display.display();                      // updates the buffer
  digitalWrite(LEDgreen_pin,LOW);         // turns off the green LED
  digitalWrite(LEDyellow_pin,LOW);        // turns off the yellow LED
  digitalWrite(LEDred_pin,LOW);           // turns off the red LED
  ledcWriteTone(CHANNEL, 0);              // turns off the buzzer

  // HERE <----------- USER INPUT
  // Inputs from the user when calibrating.
  int x_max=42.06,x_min=12.02,x_side=21.03,y_max=63.08,y_min=30.04,y_side=48.06;
  
  float m1 = (0-90)/(x_side-x_min);     // inclination factor of the line formula from 0° to 90°
  float m2 = (180-90)/(x_side-x_min);   // inclination factor of the line formula from 90° to 180°
  float b1 = 90-m1*x_min;               // computing the offset from 0° to 90°
  float b2 = 90-m2*x_min;               // computing the offset from 90° to 180°

  if(abs(magx) <= (x_side) && abs(magy) > (y_side))   // conditional to be met from 0° to 90°
  {
    servo_angle_north = int(m1*abs(magx)+b1);         // P-controller for the servo angle scaling
  }
  else if(abs(magx) <= (x_side) && abs(magy) < (y_side))  // conditional to be met from 90° to 180°
  {
    servo_angle_north = int(m2*abs(magx)+b2);         // P-controller for the servo angle scaling
  }

  // Data range delimitation for the servomotor
  if(servo_angle_north > 180)
    servo_angle_north = 180;
  else if(servo_angle_north < 0)
    servo_angle_north = 0;
    
  myservo.write(servo_angle_north); // write the computed angle to the servo

  TouchRead(CHANNEL);   // reads the touch buttons on each interation
}
