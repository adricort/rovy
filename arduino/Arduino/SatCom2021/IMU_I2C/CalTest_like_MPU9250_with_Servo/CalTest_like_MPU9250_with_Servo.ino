#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <DPEng_BMX160.h>
#include<Servo.h>
#include <STM32RTC.h>

#define DECLINATION 3.69

const int servoPin = PA0;
int servo_angle = 90;
Servo servo;

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

String frame_calibrated = "$IIMUC,";

/* Magnetometer variables:
   Variables that store magnetic field values along x,y and z axes */
static float mx,my,mz;
static float ax,ay,az;
static float Heading = 0;  // Actual heading read by magnetometer. Compared agains the sun vector.
/* Buffer holding 25 x, y and z readings for the magnetometer moving average */
//float values_x[25];
//float values_y[25];
//float values_z[25];
/* Holds the sum of 25 measurements */
float average_temp_x;
float average_temp_y;
float average_temp_z;
/* Variable used to overwrite old values in the buffer */
int counter = 0;
int max_counter_calibration = 2000;    // do 2000*delay ms the sensor.event (usually around 30 seconds)
/* Axillary variables for the low pass filter */
float filter; 
float SmoothData_x, SmoothData_y, SmoothData_z; // Readings after the low pass filter is applied ; 
float RawData_x, RawData_y, RawData_z; // Raw readings. Factory compensation values have to be applied ;
float LPF_Beta = 0.025; //0.0025; // Determines the cut-off frequency which decreases as the value -> 0 ;
float pitch;             // Pitch for magnetometer tilt compensation ;
float roll;              // Roll for magnetometer tilt compensation ;
/* Magnetometer variables for calibration */
float Offset_x;       // Offset for the initial hard iron calibration x ;
float Offset_y;       // Offset for the initial hard iron calibration  y ;
float Offset_z;       // Offset for the initial hard iron calibration  y ; 
float Offset_x_final; // Offset for the ginal hard iron calibration x ;
float Offset_y_final; // Offset for the final hard iron calibration y ;
float theta_rot;      // Angle of rotation. Used for the soft iron calibration ;
float Mag_x_scale;    // Scale for the magnetic field sensor x;
float Mag_y_scale;    // Scale for the magnetic field sensor y;


/* Additional variables used to store the accelerometer readings */
float Accel_total_vector ;       // Total magnitude of the gravitational vector ;
float Accel_x, Accel_y, Accel_z; // Hold unbiased accelerometer readings ;
float Accel_pitch;               // Holds pitch angle. Used for tilt compensation ;
float Accel_roll;                // Holds roll angle. Used for tilt compensation ;
float bias_x = 0;                // Offset for the accelerometer x axis ;
float bias_y = 0;                // Offset for the accelerometer y axis ;
float bias_z = 0;                // Offset for the accelerometer z axis ;

/* Magnetometer factory compensating values for the sensitivity adjustment
   were obtained with the following commands:
   Wire.beginTransmission(0x0C);                        // Open session with the magnetometer AK8963
   Wire.write(0x10);                                    // Point to AK8963 fuse ROM register
   Wire.endTransmission();
   Wire.requestFrom(0x0C, 3);                           // Request 3 bytes    
   ASAX = (Wire.read() - 128) * 0.5 / 128 + 1;          // Adjust readings
   ASAY = (Wire.read() - 128) * 0.5 / 128 + 1;          // Adjust readings
   ASAZ = (Wire.read() - 128) * 0.5 / 128 + 1;          // Adjust readings
// Returned values: ASAX = 1.21094; ASAY = 1.21484; ASAZ=1.16797 */
/*Factory compensation values : ASAX = 1.21094; ASAY = 1.21484; ASAZ=1.16797;

 References: 
 
  [1]Applications of Magnetic Sensors for   
  Low Cost Compass Systems
  Michael J. Caruso; Honeywell, SSEC.*/
// IT'S PENDING TO SET THE RIGHT COMPENSATING VALUES FOR SENSITIVITY ADJUSTMENT FROM BMX160
float ASAX = 1.21094;
float ASAY = 1.21484;
float ASAZ = 1.16797; //ORIGINAL! :) FROM MPU9250
//float ASAZ = 1.21216;

/* Assign a unique ID to this sensor at the same time */
DPEng_BMX160 dpEng = DPEng_BMX160(0x160A, 0x160B, 0x160C);

void displaySensorDetails(void)
{
  sensor_t accel, gyro, mag;
  dpEng.getSensor(&accel, &gyro, &mag);
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
  servo.attach(servoPin);
  servo.write(90);

  // Wait for the Serial Monitor
  while (!Serial) {
    delay(1);
  }

  // Initialise the sensor
  dpEng.begin(BMX160_ACCELRANGE_4G, GYRO_RANGE_250DPS);
  // Display some basic information on this sensor
  displaySensorDetails();

  rtc.begin(); // initialize RTC 24H format

  // you can use also
  if (!rtc.isTimeSet()) {
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(weekDay, day, month, year);
  }
}

void loop(void)
{
  Mag_hard_iron_calibration(); // Calibrate magnetoeter - hard iron calibration ;
  while(1)
  {
    Read_gyro_accel();
    Magnetic_sensor_read();
    if(servo_angle > 0 && servo_angle < 90 && Heading > 0 && Heading < 90)
      servo_angle = 0;
    else if(servo_angle > 90 && servo_angle < 180 && Heading > 90 && Heading < 180)
      servo_angle = 180;
    servo.write(abs(servo_angle));

    frame_calibrated.concat(rtc.getDay()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getMonth()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getYear()); frame_calibrated.concat(",");
    frame_calibrated.concat(rtc.getHours()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getMinutes()); frame_calibrated.concat("."); frame_calibrated.concat(rtc.getSeconds()); frame_calibrated.concat(",");
    frame_calibrated.concat(Accel_roll); frame_calibrated.concat(",");
    frame_calibrated.concat(Accel_pitch); frame_calibrated.concat(",");
    frame_calibrated.concat(Heading); frame_calibrated.concat(",");
    // Checksum ------------------------------------------------
    int frame_len = frame_calibrated.length() + 1; // Length (with one extra character for the null terminator)
    // Prepare the character array (the buffer)
    char char_array[frame_len];
    frame_calibrated.toCharArray(char_array, frame_len);
    int checksum = nmea0183_checksum(char_array);
    frame_calibrated.concat(String(checksum));
    // ---------------------------------------------------------
    Serial.println(frame_calibrated);
  
    // Reinitialization of String
    frame_calibrated = "$IIMUC,";
    
    delay(10);
  }
}
