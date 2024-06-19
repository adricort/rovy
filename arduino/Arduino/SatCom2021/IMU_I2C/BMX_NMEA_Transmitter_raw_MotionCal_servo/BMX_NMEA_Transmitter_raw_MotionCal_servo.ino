#include <Wire.h>
#include <STM32RTC.h>
#include <Adafruit_Sensor.h> 
#include <DPEng_BMX160.h>
#include<Servo.h>

// For the calibration. See more info here: https://nitinjsanket.github.io/tutorials/attitudeest/mahony
#include <Mahony_BMX160.h>
#include <Madgwick_BMX160.h>

// Mag calibration values are calculated via ahrs_calibration example sketch results.
// These values must be determined for each baord/environment.
// See the image in this sketch folder for the values used
// below.

// Offsets applied to raw x/y/z mag values
float mag_offsets[3]            = { 111.23F, -32.36F, 12.02F };

// Soft iron error compensation matrix
float mag_softiron_matrix[3][3] = { {  0.582,  -0.021,  0.034 },
                                    {  0.021,  0.676, 0.309 },
                                    {  0.033,  0.308,  2.685 } };

float mag_field_strength        = 47.60F;

// Offsets applied to compensate for gyro zero-drift error for x/y/z
float gyro_zero_offsets[3]      = { 0.0F, 0.0F, 0.0F };

// Mahony is lighter weight as a filter and should be used
// on slower systems
Mahony_BMX160 filter;
//Madgwick_BMX160 filter;

// set RX and TX pins
HardwareSerial Serial1(PA10, PA9);

/* Get the rtc object */
STM32RTC& rtc = STM32RTC::getInstance();
/* Change these values to set the current initial time */
byte seconds = 0;
const byte minutes = 23;
const byte hours = 17;

/* Change these values to set the current initial date */
/* Monday 15th June 2015 */
const byte weekDay = 1;
const byte day = 7;
const byte month = 6;
const byte year = 21;

const int servoPin = PA0;

String frame_raw = "$IIMUR,";
String frame_calibrated = "$IIMUC,";

/* Assign a unique ID to this sensor at the same time */
DPEng_BMX160 dpEng = DPEng_BMX160(0x160A, 0x160B, 0x160C);

Servo servo;

void displaySensorDetails(void)
{
  sensor_t accel, gyro, mag;
  dpEng.getSensor(&accel, &gyro, &mag);
  /*Serial.println("------------------------------------");
    Serial.println("ACCELEROMETER");
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(accel.name);
    Serial.print  ("Driver Ver:   "); Serial.println(accel.version);
    Serial.print  ("Unique ID:    0x"); Serial.println(accel.sensor_id, HEX);
    Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
    Serial.print  ("Max Value:    "); Serial.print(accel.max_value, 4); Serial.println(" m/s^2");
    Serial.print  ("Min Value:    "); Serial.print(accel.min_value, 4); Serial.println(" m/s^2");
    Serial.print  ("Resolution:   "); Serial.print(accel.resolution, 8); Serial.println(" m/s^2");
    Serial.println("------------------------------------");
    Serial.println("");
    Serial.println("------------------------------------");
    Serial.println("GYROSCOPE");
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(gyro.name);
    Serial.print  ("Driver Ver:   "); Serial.println(gyro.version);
    Serial.print  ("Unique ID:    0x"); Serial.println(gyro.sensor_id, HEX);
    Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
    Serial.print  ("Max Value:    "); Serial.print(gyro.max_value); Serial.println(" g");
    Serial.print  ("Min Value:    "); Serial.print(gyro.min_value); Serial.println(" g");
    Serial.print  ("Resolution:   "); Serial.print(gyro.resolution); Serial.println(" g");
    Serial.println("------------------------------------");
    Serial.println("");
    Serial.println("------------------------------------");
    Serial.println("MAGNETOMETER");
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(mag.name);
    Serial.print  ("Driver Ver:   "); Serial.println(mag.version);
    Serial.print  ("Unique ID:    0x"); Serial.println(mag.sensor_id, HEX);
    Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
    Serial.print  ("Max Value:    "); Serial.print(mag.max_value); Serial.println(" uTesla");
    Serial.print  ("Min Value:    "); Serial.print(mag.min_value); Serial.println(" uTesla");
    Serial.print  ("Resolution:   "); Serial.print(mag.resolution); Serial.println(" uTesla");
    Serial.println("------------------------------------");
    Serial.println("");
    delay(500);*/
}

int nmea0183_checksum(char *nmea_data) {
  int crc = 0;

  for (int i = 1; i < strlen(nmea_data); i++) {
    crc ^= nmea_data[i];
  }

  return crc;
}

void setup(void)
{
  Serial.begin(115200);     // FOR THE SERIAL MONITOR
  Serial1.begin(115200);    // FOR THE MEGA

  servo.attach(servoPin); 
  // Wait for the Serial Monitor
  while (!Serial) {
    delay(1);
  }

  // Initialise the sensor
  dpEng.begin(BMX160_ACCELRANGE_4G, GYRO_RANGE_250DPS);
  // Display some basic information on this sensor
  displaySensorDetails();

  // Select RTC clock source: LSI_CLOCK, LSE_CLOCK or HSE_CLOCK.
  // By default the LSI is selected as source.
  //rtc.setClockSource(STM32RTC::LSE_CLOCK);

  rtc.begin(); // initialize RTC 24H format

  // you can use also
  if (!rtc.isTimeSet()) {
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(weekDay, day, month, year);
  }

  filter.begin();
}

void loop(void)
{
  sensors_event_t aevent, gevent, mevent;

  // Get a new sensor event
  dpEng.getEvent(&aevent, &gevent, &mevent);

// RAW DATA -------------------------------------------------------------------
  frame_raw.concat(rtc.getDay()); frame_raw.concat("."); frame_raw.concat(rtc.getMonth()); frame_raw.concat("."); frame_raw.concat(rtc.getYear()); frame_raw.concat(",");
  frame_raw.concat(rtc.getHours()); frame_raw.concat("."); frame_raw.concat(rtc.getMinutes()); frame_raw.concat("."); frame_raw.concat(rtc.getSeconds()); frame_raw.concat(",");
  frame_raw.concat(gevent.gyro.x); frame_raw.concat(",");
  frame_raw.concat(gevent.gyro.y); frame_raw.concat(",");
  frame_raw.concat(gevent.gyro.z); frame_raw.concat(",");
  frame_raw.concat(aevent.acceleration.x); frame_raw.concat(",");
  frame_raw.concat(aevent.acceleration.y); frame_raw.concat(",");
  frame_raw.concat(aevent.acceleration.z); frame_raw.concat(",");
  frame_raw.concat(mevent.magnetic.x); frame_raw.concat(",");
  frame_raw.concat(mevent.magnetic.y); frame_raw.concat(",");
  frame_raw.concat(mevent.magnetic.z); frame_raw.concat(",");
  // Checksum ------------------------------------------------
  int frame_len = frame_raw.length() + 1; // Length (with one extra character for the null terminator)
  // Prepare the character array (the buffer)
  char char_array[frame_len];
  frame_raw.toCharArray(char_array, frame_len);
  int checksum = nmea0183_checksum(char_array);
  frame_raw.concat(String(checksum));
  // ---------------------------------------------------------
  frame_raw.concat("%");
  //Serial.println(frame_raw);
  //Serial1.println(frame_raw);
  // Reinitialization of String
  frame_raw = "$IIMUR,";
  

// CALIBRATED DATA -------------------------------------------------------------------
  // Apply mag offset compensation (base values in uTesla)
  float x = mevent.magnetic.x - mag_offsets[0];
  float y = mevent.magnetic.y - mag_offsets[1];
  float z = mevent.magnetic.z - mag_offsets[2];

  // Apply mag soft iron error compensation
  float mx = x * mag_softiron_matrix[0][0] + y * mag_softiron_matrix[0][1] + z * mag_softiron_matrix[0][2];
  float my = x * mag_softiron_matrix[1][0] + y * mag_softiron_matrix[1][1] + z * mag_softiron_matrix[1][2];
  float mz = x * mag_softiron_matrix[2][0] + y * mag_softiron_matrix[2][1] + z * mag_softiron_matrix[2][2];

  // Apply gyro zero-rate error compensation
  float gx = gevent.gyro.x + gyro_zero_offsets[0];
  float gy = gevent.gyro.y + gyro_zero_offsets[1];
  float gz = gevent.gyro.z + gyro_zero_offsets[2];

  // Update the filter
  filter.update(gx, gy, gz,
                aevent.acceleration.x, aevent.acceleration.y, aevent.acceleration.z,
                mx, my, mz,
        mevent.timestamp);

  // Print the orientation filter output
  // Note: To avoid gimbal lock you should read quaternions not Euler
  // angles, but Euler angles are used here since they are easier to
  // understand looking at the raw values. See the ble fusion sketch for
  // and example of working with quaternion data.
  float roll = filter.getRoll();
  float pitch = filter.getPitch();
  float heading = filter.getYaw();
  
  frame_calibrated.concat(rtc.getDay()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getMonth()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getYear()); frame_calibrated.concat(",");
  frame_calibrated.concat(rtc.getHours()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getMinutes()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getSeconds()); frame_calibrated.concat(",");
  frame_calibrated.concat(roll); frame_calibrated.concat(",");
  frame_calibrated.concat(pitch); frame_calibrated.concat(",");
  frame_calibrated.concat(heading); frame_calibrated.concat(",");
  // Checksum ------------------------------------------------
  frame_len = frame_calibrated.length() + 1; // Length (with one extra character for the null terminator)
  // Prepare the character array (the buffer)
  char_array[frame_len];
  frame_calibrated.toCharArray(char_array, frame_len);
  checksum = nmea0183_checksum(char_array);
  frame_calibrated.concat(String(checksum));
  // ---------------------------------------------------------
  frame_calibrated.concat("%");
  //Serial.println(frame_calibrated);
  //Serial1.println(frame_calibrated);

  // Reinitialization of String
  frame_calibrated = "$IIMUC,";

  // For testint the Servo
  //heading = heading + 90;
  if(heading > 180)
    heading = 180;
  else if(heading < -90)
    heading = 180;
  Serial.println(heading);
  servo.write(180-abs(heading));
  
  delay(10);
}
