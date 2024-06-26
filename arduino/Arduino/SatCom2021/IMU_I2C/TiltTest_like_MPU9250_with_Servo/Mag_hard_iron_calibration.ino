/**
  This function calibrates the magnetic field sensor. 
  The process includes two steps: Hard iron calibration and soft iron calibration.

  Hard iron calibration compensates the bias that is present in the sensor offseting
  the measurements back to the origin. The magnetic field 
**/


/*void Mag_hard_iron_calibration(){
  Serial.println("Mag_hard_iron_calibration has begun...");
  float Factor = 0.025;      // ORIGINALLY SET TO 0.025, IS IT BETTER OR NOT?
  // Max and min X/Y 
  //float max_x = -2000.0;
  //float max_y = -2000.0;    // ORIGINALLY SET TO -1000, IS IT BETTER OR NOT?
  //float min_x =  2000.0;    // ORIGINALLY SET TO 1000, IS IT BETTER OR NOT?
  //float min_y =  2000.0;    // ORIGINALLY SET TO 1000, IS IT BETTER OR NOT?
  float max_x = 423.20;
  float max_y = 63.09;  
  float min_x =  195.30;  
  float min_y =  -107.71;

  /// Initial values for the low pass filter fo x and y axes. 
  //   Readings are multiplied by the factory correction values.
  sensors_event_t aevent, gevent, mevent;
  dpEng.getEvent(&aevent, &gevent, &mevent);
  mx = mevent.magnetic.x;
  my = mevent.magnetic.y;
  mz = mevent.magnetic.z;

  SmoothData_x = mx * ASAX;  
  SmoothData_y = my * ASAY;

  for (int i=0; i<max_counter_calibration; i++){
     // Read raw data and apply factory compensation
    sensors_event_t aevent, gevent, mevent;
    dpEng.getEvent(&aevent, &gevent, &mevent);
    mx = mevent.magnetic.x;
    my = mevent.magnetic.y;
    mz = mevent.magnetic.z;
    RawData_x=mx * ASAX;
    RawData_y=my * ASAY;
    
    // Low pass filters for x, y and z axes with Factor = 0.025 
    SmoothData_x = SmoothData_x - (Factor * (SmoothData_x - RawData_x));
    SmoothData_y = SmoothData_y - (Factor * (SmoothData_y - RawData_y));
    
    // Store min / max x and y
    if (SmoothData_x > max_x) {max_x = SmoothData_x;}
    if (SmoothData_x < min_x) {min_x = SmoothData_x;}
    if (SmoothData_y > max_y) {max_y = SmoothData_y;}
    if (SmoothData_y < min_y) {min_y = SmoothData_y;}
     
    // Delay between each value
    delay(10);
    Serial.print("Calibrating until: ");Serial.print(max_counter_calibration - i);  
    Serial.print(" max_x: ");Serial.print(max_x);
    Serial.print(" max_y: ");Serial.print(max_y);
    Serial.print(" min_x: ");Serial.print(min_x);
    Serial.print(" min_y: ");Serial.println(min_y);
  }
  Offset_x = (max_x+min_x)/2;
  Offset_y = (max_y+min_y)/2;
  mx = mevent.magnetic.x;
  my = mevent.magnetic.y;
  mz = mevent.magnetic.z;

 // Apply factory compensation values 
  SmoothData_x = mx * ASAX;
  SmoothData_y = my * ASAY;
  SmoothData_z = mz * ASAZ;

  // Apply scaling and offset the reading to the origin
  SmoothData_x = SmoothData_x - Offset_x;
  SmoothData_y = SmoothData_y - Offset_y;
  
  Serial.println("Hard iron calibration has been completed");
  Serial.print("SmoothData_x: ");Serial.print(SmoothData_x);
  Serial.print(" SmoothData_y: ");Serial.print(SmoothData_y);
  Serial.print(" SmoothData_z: ");Serial.print(SmoothData_z);
  Serial.print(" Offset_x: ");Serial.print(Offset_x);
  Serial.print(" Offset_y: ");Serial.println(Offset_y);
}
*/
