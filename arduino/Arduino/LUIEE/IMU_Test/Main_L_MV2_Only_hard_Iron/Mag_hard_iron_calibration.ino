/**
  This function calibrates the magnetic field sensor. 
  The process includes two steps: Hard iron calibration and soft iron calibration.

  Hard iron calibration compensates the bias that is present in the sensor offseting
  the measurements back to the origin. The magnetic field 
**/


void Mag_hard_iron_calibration(){
  Serial.println("Mag_hard_iron_calibration has begun...");
  float Factor = 0.15;
  /* Max and min X/Y */
  float max_x = -1000.0;
  float max_y = -2000.0;
  float min_x =  1100.0;
  float min_y =  2200.0;

  fabo_9axis.begin();

  /* Initial values for the low pass filter fo x and y axes. 
     Readings are multiplied by the factory correction values. */
  fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
  Serial.println(mx);
  SmoothData_x = mx * ASAX;  
  SmoothData_y = my * ASAY;

  /* The magnetic field is going to be recorded for 1 whole rotation */
  for ( int Steps = 0; Steps < 200; Steps ++) {
    //Serial.print("Mag_hard_iron_calibration");Serial.print(Steps/2);Serial.println("%");
    /* Delay between each step */
    delay(70);
    
    //////////////////////////////////
    ////// Nema 1 full step add //////
    //////////////////////////////////
  
   /* Read the magnetic field 25 times */
    for (int ii=0; ii<25; ii++){

      /* Read raw data and apply factory compensation */
      fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
      RawData_x=mx * ASAX;
      RawData_y=my * ASAY;
      Serial.println(mx);
      
      /* Low pass filters for x, y and z axes with Factor = 0.025  */
      SmoothData_x = SmoothData_x - (Factor * (SmoothData_x - RawData_x));
      SmoothData_y = SmoothData_y - (Factor * (SmoothData_y - RawData_y));
      
      /* Store min / max x and y */
      if (SmoothData_x > max_x) {max_x = SmoothData_x;}
      if (SmoothData_x < min_x) {min_x = SmoothData_x;}
      if (SmoothData_y > max_y) {max_y = SmoothData_y;}
      if (SmoothData_y < min_y) {min_y = SmoothData_y;}         
    }
    Serial.print(SmoothData_x); Serial.print(' ');   Serial.println(SmoothData_y);   
  }
      /** Perform 1st hard iron calibration. These two results 
         are used for soft iron calibration. **/
      Offset_x = (max_x+min_x)/2;
      Offset_y = (max_y+min_y)/2;
     Serial.println("Hard iron calibration has been completed");
     Serial.println(Offset_x);
     Serial.println(Offset_y);
}
