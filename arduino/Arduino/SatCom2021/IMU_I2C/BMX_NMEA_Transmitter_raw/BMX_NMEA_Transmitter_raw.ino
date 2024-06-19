#include <Wire.h>
#include <STM32RTC.h>
#include <Adafruit_Sensor.h>
#include <DPEng_BMX160.h>

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

String frame = "$IIMUR,";

/* Assign a unique ID to this sensor at the same time */
DPEng_BMX160 dpEng = DPEng_BMX160(0x160A, 0x160B, 0x160C);

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
}

void loop(void)
{
  sensors_event_t aevent, gevent, mevent;

  // Get a new sensor event
  dpEng.getEvent(&aevent, &gevent, &mevent);

  //frame.concat(rtc.getDay()); frame.concat("."); frame.concat(rtc.getMonth()); frame.concat("."); frame.concat(rtc.getYear()); frame.concat(",");
  //frame.concat(rtc.getHours()); frame.concat("."); frame.concat(rtc.getMinutes()); frame.concat("."); frame.concat(rtc.getSeconds()); frame.concat(",");
  frame.concat(gevent.gyro.x); frame.concat(",");
  frame.concat(gevent.gyro.y); frame.concat(",");
  frame.concat(gevent.gyro.z); frame.concat(",");
  frame.concat(aevent.acceleration.x); frame.concat(",");
  frame.concat(aevent.acceleration.y); frame.concat(",");
  frame.concat(aevent.acceleration.z); frame.concat(",");
  frame.concat(mevent.magnetic.x); frame.concat(",");
  frame.concat(mevent.magnetic.y); frame.concat(",");
  frame.concat(mevent.magnetic.z); frame.concat(",");
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

  delay(50);
}
