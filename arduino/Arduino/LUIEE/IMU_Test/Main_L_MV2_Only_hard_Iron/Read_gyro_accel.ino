
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


void Read_gyro_accel() {

  while (1){  
    
/* Read accelerometer (library) */
  fabo_9axis.readAccelXYZ(&ax,&ay,&az);
  
  Accel_x = ax-bias_x; // Unbiased acceleration along x axis ;
  Accel_y = ay-bias_y; // Unbiased acceleration along y axis ;
  Accel_z = az-bias_z; // Unbiased acceleration along z axis ;
  //Serial.print("Accel_x");Serial.println(Accel_x,5);
  //Serial.print("Accel_y");Serial.println(Accel_y,5);
  //Serial.print("Accel_z");Serial.println(Accel_z,5);


/* Determine pitch and roll angles*/
  Accel_total_vector = sqrt((Accel_x*Accel_x) + (Accel_y*Accel_y) + (Accel_z*Accel_z));// Calculate the total magnitude
  Accel_pitch = asin((float)Accel_x/Accel_total_vector) * RAD_TO_DEG;                  //Calculate the pitch angle
  Accel_roll = asin((float)Accel_y/Accel_total_vector) * RAD_TO_DEG;                   //Calculate the roll angle
  
  //Serial.print("Accel_pitch");Serial.println(Accel_pitch);
  //Serial.print("Accel_roll");Serial.println(Accel_roll);

  /* Adjustments for pitch and roll */
  Accel_pitch = Accel_pitch - 0.0;
  Accel_roll  = Accel_roll  - 0.0;
  
  delay(350);

  /* Obtain gyroscope readins(library) */
//fabo_9axis.readGyroXYZ(&gx,&gy,&gz);
  
  }
  
}

/* Define variables and configure gyro / accelerometer  */

/*float angle_gyro = 0;
  int     Gyro_x,     Gyro_y,     Gyro_z;
  long    Gyro_x_cal, Gyro_y_cal, Gyro_z_cal;
  float   Gyro_pitch, Gyro_roll, Gyro_yaw;
  float   Gyro_pitch_output, Gyro_roll_output;
  long    Accel_x, Accel_y, Accel_z;*/

  /* Activate the MPU-6050 */
 /* Wire.beginTransmission(0x68);                         //Open session with the MPU-6050
  Wire.write(0x6B);                                     //Point to power management register
  Wire.write(0x00);                                     //Use internal 20MHz clock
  Wire.endTransmission();                               //End the transmission*/

  /* Configure gyroscope */
 /* Wire.beginTransmission(0x68);                         //Open session with the MPU-6050
  Wire.write(0x1B);                                     //Point to gyroscope configuration
  Wire.write(0x08);                                     //Select 500dps full-scale
  Wire.endTransmission(); */

  /* Configure accelerometer */
  /*Wire.beginTransmission(0x68);                         //Open session with the MPU-6050
  Wire.write(0x1B);                                     //Point to gyroscope configuration
  Wire.write(0x8);                                     //Select 500dps full-scale
  Wire.endTransmission(); */ 

 /* Get gyroscope and accelerometer readings manually: point to the 1st data register */
/*Wire.beginTransmission(0x68);                         // Start communicating with the MPU-6050
  Wire.write(0x3B);                                     // Point to start of data
  Wire.endTransmission();                               // End the transmission*/

/* Read accelerometer, temperature and gyroscope values  */
 /* Wire.requestFrom(0x68, 14);                         // Request 14 bytes from the MPU-6050
  while (Wire.available() < 14);   {                    // Wait until all the bytes are received
  Accel_x = Wire.read() << 8 | Wire.read();             // Combine MSB,LSB Accel_x variable
  Accel_y = Wire.read() << 8 | Wire.read();             // Combine MSB,LSB Accel_y variable
  Accel_z = Wire.read() << 8 | Wire.read();             // Combine MSB,LSB Accel_z variable
  int  temperature = Wire.read() << 8 | Wire.read();    // Combine MSB,LSB temperature variable
  Gyro_x = Wire.read() << 8 | Wire.read();              // Combine MSB,LSB Gyro_x variable
  Gyro_y = Wire.read() << 8 | Wire.read();              // Combine MSB,LSB Gyro_x variable
  Gyro_z = Wire.read() << 8 | Wire.read(); }            // Combine MSB,LSB Gyro_x variable
  Serial.println(gx);
  Serial.println(gy);
  Serial.println(gz);
  
  //angle_gyro = angle_gyro + gy*0.04;
  //Serial.println(angle_gyro);
  delay(200);
  //Serial.println(Accel_z);*/
