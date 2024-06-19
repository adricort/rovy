/* 
   Inputs and outputs for the functions: 
   
   Mag_hard_iron_calibration  -> [Offset_x Offset_y] -> Mag_soft_iron_calibration
   Mag_soft_iron_calibration  -> [theta_rot]         -> Mag_scaling
   Mag_scaling                -> [Mag_x_scale Mag_y_scale Offset_x_final Offset_y_final] -> Mag_sensor_read
*/
void Magnetic_sensor_read()
{  
  sensors_event_t aevent, gevent, mevent;

  /* Read raw data for x, y and z axes and include factory 
    compensation values */
  dpEng.getEvent(&aevent, &gevent, &mevent);
  mx = mevent.magnetic.x;
  my = mevent.magnetic.y;
  
 /* Apply factory compensation values */
  RawData_x = mx * ASAX;
  RawData_y = my * ASAY;

 /* Apply scaling and offset the reading to the origin */
  //RawData_x = RawData_x * Mag_x_scale - Offset_x_final;   // ORIGINAL WHEN DOING MAG_SCALING FUNCTION
  //RawData_y = RawData_y * Mag_x_scale - Offset_x_final;   // ORIGINAL WHEN DOING MAG_SCALING FUNCTION
  RawData_x = RawData_x - Offset_x;
  RawData_y = RawData_y - Offset_y;
 
 /* Low pass filters for x, y and z axes with LPF_Beta = 0.0025  */
  SmoothData_x = SmoothData_x - (LPF_Beta * (SmoothData_x - RawData_x));
  SmoothData_y = SmoothData_y - (LPF_Beta * (SmoothData_y - RawData_y));
  
  /* Implementation of tilt compensation 
  In the following formula roll is θ and pitch is φ:  
  Xh = X*cos(φ) + Y*sin(θ)*sin(φ) - Z*cos(θ)*sin(φ);
  Yh = Y*cos(θ) + Z*sin(θ); */

  /* Obtain pitch and roll readings from the acceleromter */
     //Read_gyro_accel();
    
  /* Store pitch and roll readings into magetometer-specific variables*/
   //pitch = Accel_pitch;
   //roll  = Accel_roll;
  
  //Xh = mx*cos(pitch) + my*sin(roll)*sin*(pitch) - mz*cos(roll)*sin(pitch);
  //Yh = my*cos(roll) + mz*sin(roll) ;
  /* Determine the heading */
  Heading = atan2(SmoothData_x, SmoothData_y) * RAD_TO_DEG;  // Magnetic North ADD DECLINATION
  //Heading -= DECLINATION;    // IS IT + OR -?
  servo_angle = Heading;
  if(Heading < 0)
    Heading += 360;

  /** Calculate the arithmetic average of the heading for 50 readings. **/
  //Heading = Heading / 50.0; 
  //Serial.print("Final heading average:");Serial.println(Heading,5);
  //delay(5000) ;
}
