
/* 
 This function obtains roll and pitch angles that 
 are used later for tilt compensation of magnetometer.
 
 TODO: -- Add low pass filter 
       -- Re-calibrate sensor
       -- Add 2nd calibration values


 Accelerometer Bias:
 
 bias_x = -0.06102
 bias_y = 0.18034
 bias_z = 0.82496

                     
*/

/* Calibration routine for accelerometer (eliminates the bias)
  for (int ii=0; ii<2000; ii++) {
  fabo_9axis.readAccelXYZ(&ax,&ay,&az);
  temp_readings = temp_readings + ax;
  delay(10);
  }
  bias_x =1.0 - temp_readings/2000.0;
  Serial.print("bias_x");Serial.println(bias_x,5);
  Serial.print("temp_reading");Serial.println(temp_readings/2000.0);
  delay(2000);*/


void Read_gyro_accel()
{
  sensors_event_t aevent, gevent, mevent;
  dpEng.getEvent(&aevent, &gevent, &mevent);

  ax = aevent.acceleration.x;
  ay = aevent.acceleration.y;
  az = aevent.acceleration.z;
    
  Accel_x = ax-bias_x; // Unbiased acceleration along x axis ;
  Accel_y = ay-bias_y; // Unbiased acceleration along y axis ;
  Accel_z = az-bias_z; // Unbiased acceleration along z axis ;

/* Determine pitch and roll angles*/

  Accel_roll = atan(Accel_x/sqrt(Accel_y*Accel_y+Accel_z*Accel_z))* RAD_TO_DEG; //asin((float)Accel_x/Accel_total_vector) * RAD_TO_DEG;                  //Calculate the pitch angle
  Accel_pitch = - atan(Accel_y/sqrt(Accel_x*Accel_x+Accel_z*Accel_z))* RAD_TO_DEG; // asin((float)Accel_y/Accel_total_vector) * RAD_TO_DEG;                   //Calculate the roll angle

  /* Adjustments for pitch and roll */
  Accel_pitch = Accel_pitch + 1.1;
  Accel_roll  = Accel_roll  - 0.15;
}
