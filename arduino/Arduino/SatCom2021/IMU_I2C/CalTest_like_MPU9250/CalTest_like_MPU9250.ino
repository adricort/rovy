#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <DPEng_BMX160.h>

#define DECLINATION 3.69

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
int max_counter_calibration = 1000;    // do 1000*delay ms the sensor.event (usually around 30 seconds)
/* Axillary variables for the low pass filter */
float filter; 
float SmoothData_x, SmoothData_y, SmoothData_z; // Readings after the low pass filter is applied ; 
float RawData_x, RawData_y, RawData_z; // Raw readings. Factory compensation values have to be applied ;
float LPF_Beta = 0.0025; // Determines the cut-off frequency which decreases as the value -> 0 ;
float pitch;             // Pitch for magnetometer tilt compensation ;
float roll;              // Roll for magnetometer tilt compensation ;
/* Magnetometer variables for calibration */
float Offset_x;       // Offset for the initial hard iron calibration x ;
float Offset_y;       // Offset for the initial hard iron calibration  y ; 
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
float ASAZ = 1.16797;

/* Assign a unique ID to this sensor at the same time */
DPEng_BMX160 dpEng = DPEng_BMX160(0x160A, 0x160B, 0x160C);

void displaySensorDetails(void)
{
  sensor_t accel, gyro, mag;
  dpEng.getSensor(&accel, &gyro, &mag);
}

void setup(void)
{
  Serial.begin(115200);     // FOR THE SERIAL MONITOR

  // Wait for the Serial Monitor
  while (!Serial) {
    delay(1);
  }

  // Initialise the sensor
  dpEng.begin(BMX160_ACCELRANGE_4G, GYRO_RANGE_250DPS);
  // Display some basic information on this sensor
  displaySensorDetails();
}

void loop(void)
{
  Mag_hard_iron_calibration(); // Calibrate magnetoeter - hard iron calibration ;
  while(1)
  {
    Magnetic_sensor_read();
    Read_gyro_accel();
    Serial.print("Heading: ");Serial.print(Heading,2);
    Serial.print(" Pitch: ");Serial.print(Accel_pitch,2);
    Serial.print(" Roll: ");Serial.println(Accel_roll,2);
    delay(10);
  }
}
