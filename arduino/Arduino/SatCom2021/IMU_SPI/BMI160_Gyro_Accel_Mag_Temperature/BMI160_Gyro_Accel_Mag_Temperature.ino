#include <Wire.h>         // <----------------- (TO BE REMOVED IN THE FUTURE)
#include <STM32RTC.h>
#include <BMI160Gen.h>
#include <DPEng_BMX160.h> // <----------------- (TO BE REMOVED IN THE FUTURE)

// set RX and TX pins for UART communication
HardwareSerial Serial1(PA10, PA9);

// Get the rtc object 
STM32RTC& rtc = STM32RTC::getInstance();

// Change these values to set the current initial time 
byte seconds = 0;
const byte minutes = 23;
const byte hours = 17;

// Change these values to set the current initial date 
const byte weekDay = 1;
const byte day = 7;
const byte month = 6;
const byte year = 21;

// Initialization of String (NMEA sentence)
String frame = "$IIMUR,";

// Assign a unique ID to this sensor at the same time <----------------- (TO BE REMOVED IN THE FUTURE)
DPEng_BMX160 dpEng = DPEng_BMX160(0x160A, 0x160B, 0x160C);

void setup() {
  Serial.begin(115200); // initialize Serial communication
  Serial1.begin(115200);    // FOR THE MEGA
  while (!Serial);    // wait for the serial port to open

  // Initialise the sensor <----------------- (TO BE REMOVED IN THE FUTURE)
  dpEng.begin(BMX160_ACCELRANGE_4G, GYRO_RANGE_250DPS);

  // Select RTC clock source: LSI_CLOCK, LSE_CLOCK or HSE_CLOCK.
  // By default the LSI is selected as source.
  //rtc.setClockSource(STM32RTC::LSE_CLOCK);
  rtc.begin(); // initialize RTC 24H format

  // you can use also
  if (!rtc.isTimeSet()) {
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(weekDay, day, month, year);
  }

  // Initialize BMI160
  BMI160.begin(10);

  // Set the gyroscope range to 250 degrees/second
  BMI160.setGyroRange(250);

  // Set the accelerometer range to 4 g
  BMI160.setAccelerometerRange(4);
}

void loop() {
  // variable creation for the BMI160
  int gxRaw, gyRaw, gzRaw, axRaw, ayRaw, azRaw, mxRaw, myRaw, mzRaw, tRaw;
  float gx_BMI160, gy_BMI160, gz_BMI160, ax_BMI160, ay_BMI160, az_BMI160, mx_BMI160, my_BMI160, mz_BMI160, t_BMI160;
  
  // variable creation for the BMX160 (magnetometer I2C) <----------------- (TO BE REMOVED IN THE FUTURE)
  sensors_event_t accelevent_BMX160, gyroevent_BMX160, magevent_BMX160;

  // read raw accel, gyro, and mag values from BMX160 <----------------- (TO BE REMOVED IN THE FUTURE)
  dpEng.getEvent(&accelevent_BMX160, &gyroevent_BMX160, &magevent_BMX160);
  int mx_BMX160 = magevent_BMX160.magnetic.x;
  int my_BMX160 = magevent_BMX160.magnetic.y;
  int mz_BMX160 = magevent_BMX160.magnetic.z;
  
  // read raw accel, gyro, mag, and temperature values from BMI160
  BMI160.readMotionSensor9(axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw, mxRaw, myRaw, mzRaw, tRaw);
  tRaw = BMI160.readTemperature();
  
  // convert the raw gyro data to degrees/second
  gx_BMI160 = convertRawGyro(gxRaw);
  gy_BMI160 = convertRawGyro(gyRaw);
  gz_BMI160 = convertRawGyro(gzRaw);

  // convert the raw accel data to g
  ax_BMI160 = convertRawAccel(axRaw);
  ay_BMI160 = convertRawAccel(ayRaw);
  az_BMI160 = convertRawAccel(azRaw);

  // convert the raw temp data to degrees C
  t_BMI160 = convertRawTemp(tRaw);
  
  frame.concat(rtc.getDay()); frame.concat("."); frame.concat(rtc.getMonth()); frame.concat("."); frame.concat(rtc.getYear()); frame.concat(",");
  frame.concat(rtc.getHours()); frame.concat("."); frame.concat(rtc.getMinutes()); frame.concat("."); frame.concat(rtc.getSeconds()); frame.concat(",");
  frame.concat(gx_BMI160); frame.concat(",");
  frame.concat(gy_BMI160); frame.concat(",");
  frame.concat(gz_BMI160); frame.concat(",");
  frame.concat(ax_BMI160); frame.concat(",");
  frame.concat(ay_BMI160); frame.concat(",");
  frame.concat(az_BMI160); frame.concat(",");
  frame.concat(mx_BMX160); frame.concat(",");
  frame.concat(my_BMX160); frame.concat(",");
  frame.concat(mz_BMX160); frame.concat(",");
  frame.concat(t_BMI160); frame.concat(",");
  // Checksum ------------------------------------------------
  int frame_len = frame.length() + 1; // Length (with one extra character for the null terminator)
  // Prepare the character array (the buffer)
  char char_array[frame_len];
  frame.toCharArray(char_array, frame_len);
  int checksum = nmea0183_checksum(char_array);
  frame.concat(String(checksum));
  // ---------------------------------------------------------

  frame.concat("%");
  Serial.println(frame);
  Serial1.println(frame);

  // Reinitialization of String
  frame = "$IIMUR,";

  delay(10);
}

int nmea0183_checksum(char *nmea_data) {
  int crc = 0;
  for (int i = 1; i < strlen(nmea_data); i++)
    crc ^= nmea_data[i];

  return crc;
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
  float t = tRaw * 205.9358 / 32768.0;

  return t;
}
