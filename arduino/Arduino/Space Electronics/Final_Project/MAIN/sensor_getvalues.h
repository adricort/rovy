// Function that gets the LSM6DS33 values
void LSM6DS33_getvalues(float & gyrox,float & gyroy,float & gyroz,float & accelx,float & accely,float & accelz){
  // Get a new normalized sensor event
  sensors_event_t gyro;     // gyroscope
  sensors_event_t accel;    // accelerometer
  sensors_event_t temp;     // temperature
  lsm6ds33.getEvent(&accel, &gyro, &temp);  // computing and assigning values

  // Display rotation measured in °/s
  gyrox = gyro.gyro.x*180/3.141592;   // x axis
  gyroy = gyro.gyro.y*180/3.141592;   // y axis
  gyroz = gyro.gyro.z*180/3.141592;   // z axis
  
  // Get acceleration measured in g
  accelx = accel.acceleration.x/9.81; // x axis
  accely = accel.acceleration.y/9.81; // y axis
  accelz = accel.acceleration.z/9.81; // z axis
}

// Function that gets the BME280 values
void BME280_getvalues(float & temp, float & hum, float & pres){
  // Method to get the temp, hum, and pres data
  temp = bme.readTemperature();       // Temperature in °C
  hum = bme.readHumidity();           // Percentage of humidity
  pres = bme.readPressure()/100.0F;   // Pressure in hPa
  // altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);  // Altitude in m
}

// Function that gets the MLX90393 values
void MLX90393_getvalues(int & magx,int & magy,int & magz){
  // Event method to get the magnetic field data
  sensors_event_t event;    // getting event
  sensor.getEvent(&event);  // asigning and computing event

  // Get magnetic field in uT
  magx = event.magnetic.x;    // x axis
  magy = event.magnetic.y;    // y axis
  magz = event.magnetic.z;    // z axis
}

// Function that gets the servo values
void Servo_getvalues(int & sang){
  sang = myservo.read()-90;   // Get servo angle in ° from -90° to 90°
}

// Function that gets the LDR values
void LDR_getvalues(int & ldr){
  ldr = analogRead(LDR_pin);    // Get light intensity from 0 - 4095 [resolution]
}
