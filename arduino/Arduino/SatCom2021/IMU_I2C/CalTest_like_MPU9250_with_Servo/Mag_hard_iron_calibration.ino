/**
  This function calibrates the magnetic field sensor. 
  The process includes two steps: Hard iron calibration and soft iron calibration.

  Hard iron calibration compensates the bias that is present in the sensor offseting
  the measurements back to the origin. The magnetic field 
**/


void Mag_hard_iron_calibration(){
  Serial.println("Mag_hard_iron_calibration has begun...");
  float Factor = 0.025;      // ORIGINALLY SET TO 0.025, IS IT BETTER OR NOT?
  /* Max and min X/Y */
  float max_x = -2000.0;
  float min_x =  2000.0;  
  float max_y = -2000.0; 
  float min_y =  2000.0;
  float max_z = -2000.0; 
  float min_z =  2000.0;
  /*float max_x = 422.02;
  float min_x =  209.76; 
  float max_y = 79.67;   
  float min_y =  -105.31;
  float max_z = 97.58;   
  float min_z =  -62.28;*/

  /* Initial values for the low pass filter fo x and y axes. 
     Readings are multiplied by the factory correction values. */
  sensors_event_t aevent, gevent, mevent;
  dpEng.getEvent(&aevent, &gevent, &mevent);
  mx = mevent.magnetic.x;
  my = mevent.magnetic.y;
  mz = mevent.magnetic.z;

  SmoothData_x = mx * ASAX;  
  SmoothData_y = my * ASAY;
  SmoothData_z = mz * ASAZ;

  for (int i=0; i<max_counter_calibration; i++){
     /* Read raw data and apply factory compensation */
    sensors_event_t aevent, gevent, mevent;
    dpEng.getEvent(&aevent, &gevent, &mevent);
    mx = mevent.magnetic.x;
    my = mevent.magnetic.y;
    mz = mevent.magnetic.z;
    RawData_x=mx * ASAX;
    RawData_y=my * ASAY;
    RawData_z=mz * ASAZ;
    
    /* Low pass filters for x, y and z axes with Factor = 0.025  */
    SmoothData_x = SmoothData_x - (Factor * (SmoothData_x - RawData_x));
    SmoothData_y = SmoothData_y - (Factor * (SmoothData_y - RawData_y));
    SmoothData_z = SmoothData_z - (Factor * (SmoothData_z - RawData_z));
    
    /* Store min / max x and y */
    if (SmoothData_x > max_x) {max_x = SmoothData_x;}
    if (SmoothData_x < min_x) {min_x = SmoothData_x;}
    if (SmoothData_y > max_y) {max_y = SmoothData_y;}
    if (SmoothData_y < min_y) {min_y = SmoothData_y;}
    //if (SmoothData_z > max_z) {max_z = SmoothData_z;}
    //if (SmoothData_z < min_z) {min_z = SmoothData_z;}
     
    /* Delay between each value */
    delay(10);
    Serial.print("Calibrating until: ");Serial.print(max_counter_calibration - i);  
    Serial.print(" max_x: ");Serial.print(max_x);
    Serial.print(" min_x: ");Serial.print(min_x);
    Serial.print(" max_y: ");Serial.print(max_y);
    Serial.print(" min_y: ");Serial.print(min_y);
    //Serial.print(" max_z: ");Serial.print(max_z);
    //Serial.print(" min_z: ");Serial.print(min_z);
    Serial.print(" SmoothData_z: ");Serial.println(SmoothData_z);
    delay(1);
  }
  Offset_x = (max_x+min_x)/2;
  Offset_y = (max_y+min_y)/2;
  Offset_z = (max_z+min_z)/2;
  mx = mevent.magnetic.x;
  my = mevent.magnetic.y;
  mz = mevent.magnetic.z;

 /* Apply factory compensation values */
  SmoothData_x = mx * ASAX;
  SmoothData_y = my * ASAY;
  SmoothData_z = mz * ASAZ;

  /* Apply scaling and offset the reading to the origin */
  SmoothData_x = SmoothData_x - Offset_x;
  SmoothData_y = SmoothData_y - Offset_y;
  SmoothData_z = SmoothData_z - Offset_z;
  
  Serial.println("Hard iron calibration has been completed");
  Serial.print(" Offset_x: ");Serial.print(Offset_x);
  Serial.print(" Offset_y: ");Serial.print(Offset_y);
  Serial.print(" Offset_z: ");Serial.println(Offset_z); 
}
