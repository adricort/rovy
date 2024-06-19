
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


/*void Read_gyro_accel()
{
  read_bmx160_data();
    
  Accel_x = Accel_x-bias_x; // Unbiased acceleration along x axis ;
  Accel_y = Accel_y-bias_y; // Unbiased acceleration along y axis ;
  Accel_z = Accel_z-bias_z; // Unbiased acceleration along z axis ;

// Determine pitch and roll angles

  Accel_roll = atan(Accel_x/sqrt(Accel_y*Accel_y+Accel_z*Accel_z))* RAD_TO_DEG; //asin((float)Accel_x/Accel_total_vector) * RAD_TO_DEG;                  //Calculate the pitch angle
  Accel_pitch = - atan(Accel_y/sqrt(Accel_x*Accel_x+Accel_z*Accel_z))* RAD_TO_DEG; // asin((float)Accel_y/Accel_total_vector) * RAD_TO_DEG;                   //Calculate the roll angle

  // Adjustments for pitch and roll 
  Accel_pitch = Accel_pitch + 1.1;
  Accel_roll  = Accel_roll  - 0.15;
}

void gyro_calibration()
{
  // ----- Calibrate gyro
  for (int counter = 0; counter < 2000 ; counter ++)    //Run this code 2000 times
  {
    read_mpu_6050_data();                               //Read the raw acc and gyro data from the MPU-6050
    Gyro_x_cal += Gyro_x;                               //Add the gyro x-axis offset to the gyro_x_cal variable
    Gyro_y_cal += Gyro_y;                               //Add the gyro y-axis offset to the gyro_y_cal variable
    Gyro_z_cal += Gyro_z;                               //Add the gyro z-axis offset to the gyro_z_cal variable
    while (micros() - Loop_start < Loop_time);           // Wait until "Loop_time" microseconds have elapsed
  }
  Gyro_x_cal /= 2000;                                   //Divide the gyro_x_cal variable by 2000 to get the average offset
  Gyro_y_cal /= 2000;                                   //Divide the gyro_y_cal variable by 2000 to get the average offset
  Gyro_z_cal /= 2000;                                   //Divide the gyro_z_cal variable by 2000 to get the average offset

}*/
