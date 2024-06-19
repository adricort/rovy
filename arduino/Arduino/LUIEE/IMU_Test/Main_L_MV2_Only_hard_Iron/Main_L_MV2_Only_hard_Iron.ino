/**
 file read9axis.ino
   http://fabo.io/202.html
   Released under APACHE LICENSE, VERSION 2.0
   http://www.apache.org/licenses/
 @author FaBo<info@fabo.io>
**/

#include <Wire.h>

/* Library for the small stepper motor. Used to control the position of the plane mirror */
#include <Stepper.h>

/* Add library and create an instance of the sensor object */
#include <FaBo9Axis_MPU9250.h>
FaBo9Axis fabo_9axis;         //  CONSIDER MAKING THE OBJECT LOCAL

/* Variables used for determination of the sun vector */
#define DEG_TO_RAD 0.01745329
#define PI 3.141592654
#define TWOPI 6.28318531

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
float ASAX = 1.21094;
float ASAY = 1.21484; 
float ASAZ = 1.16797;


/* Initialize the serial monitor and fill initial buffer and average */
void setup() {

 /* Set up MPU-9250 and GPS is done in the sensor specific routines
    to avoid interference between them. */
  Serial.begin(115200);
}

 /* Main loop */
void loop() {
//Obtain_GPS_readings();       // Obtain GPS readings: time and location ;
//Obtain_solar_vector();        // Determine the solar vector azimuth and elevation based on time and location ;
///Calibrate_accelerometer();   // Calibrate the accelerometer for tilt compensation ;
Mag_hard_iron_calibration(); // Calibrate magnetoeter - hard iron calibration ;
      //{Adjust_azimuth()       // Rotate the assembly and determine the offsets ;
      //}
//Mag_soft_iron_calibration();  // Determine the angle of rotation ;
      //{Adjust_azimuth()       // Rotate the assembly and determine the destance from the origin ;
      //}
//Mag_scaling() ;               // Partially normalize the ellipse to obtain uniform magnetic field readings;
      //{Adjust_azimuth()       // Rotate the assembly and determine the maximum and minimum readings ;
      //}
       // Serial.println("Magnetic sensor read has started...");
      while (1){
Magnetic_sensor_read();      // Obtain the heading based on the magnetic field readings ;
     /////////////////////////////////////////////////////////////////{Read_gyro_accel();     // Read pitch and roll angles ;
     }
    //  }
//Handle_azimuth();
      //{Adjust_azimuth(int Steps, byte Direction)
      //}

delay(100000);
}
