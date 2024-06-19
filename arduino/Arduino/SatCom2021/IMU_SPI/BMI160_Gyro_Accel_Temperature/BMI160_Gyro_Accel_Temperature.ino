#include <BMI160Gen.h>

void setup() {
  Serial.begin(9600); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device
  BMI160.begin(10);
  uint8_t dev_id = BMI160.getDeviceID();
  Serial.print("DEVICE ID: ");
  Serial.println(dev_id, HEX);

   // Set the accelerometer range to 250 degrees/second
  BMI160.setGyroRange(250);
  BMI160.setAccelerometerRange(4);
}

void loop() {
  int gxRaw, gyRaw, gzRaw, axRaw, ayRaw, azRaw, mxRaw, myRaw, mzRaw, tRaw;         // raw gyro values
  float gx, gy, gz, ax, ay, az, t;

  // read raw gyro measurements from device
  // read raw gyro measurements from device
  BMI160.readMotionSensor9(axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw, mxRaw, myRaw, mzRaw, tRaw);
  //BMI160.readGyro(gxRaw, gyRaw, gzRaw);
  //BMI160.readAccelerometer(axRaw);
  //t = BMI160.readTemperature();
  
  // convert the raw gyro data to degrees/second
  gx = convertRawGyro(gxRaw);
  gy = convertRawGyro(gyRaw);
  gz = convertRawGyro(gzRaw);

  // convert the raw accel data to g
  ax = convertRawAccel(axRaw);
  ay = convertRawAccel(ayRaw);
  az = convertRawAccel(azRaw);

  t = convertRawTemp(tRaw);
  
  // display tab-separated gyro x/y/z values
  Serial.print("gx: ");
  Serial.print(gx);
  Serial.print(" gy: ");
  Serial.print(gy);
  Serial.print(" gz: ");
  Serial.print(gz);
  Serial.print(" ax: ");
  Serial.print(ax);
  Serial.print(" ay: ");
  Serial.print(ay);
  Serial.print(" az: ");
  Serial.print(az);
  Serial.print("\t");
  Serial.print(mx);
  Serial.print(" ay: ");
  Serial.print(my);
  Serial.print(" az: ");
  Serial.print(mz);
  Serial.print("\t");
  Serial.print(t);
  Serial.println();

  delay(10);
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767

  float g = (gRaw * 250.0) / 32768.0;

  return g;
}

float convertRawAccel(int aRaw) {
  float a = (aRaw * (4/32768.0));

  return a;
}

float convertRawTemp(int tRaw) {
  float t = tRaw;

  return t;
}
