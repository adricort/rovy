// ITS PENDING TO OBTAIN PROPERLY ASAX ASAY AND ASAXZ

#include <Wire.h>
#include <Adafruit_Sensor.h> 
#include <DPEng_BMX160.h>

/* Assign a unique ID to this sensor at the same time */
DPEng_BMX160 dpEng = DPEng_BMX160(0x160A, 0x160B, 0x160C);

#define Frequency 125                       // 8mS sample interval 
#define Sensitivity 62.5                    // Gyro sensitivity (see data sheet)

//#define Frequency 250                                                   // 4mS sample interval 
//#define Sensitivity 131.2                                               // Gyro sensitivity (see data sheet)

#define Sensor_to_deg 1/(Sensitivity*Frequency)                         // Convert sensor reading to degrees
#define Sensor_to_rad Sensor_to_deg*DEG_TO_RAD                          // Convert sensor reading to radians

#define Loop_time 1000000/Frequency                                     // Loop time (uS)
long    Loop_start;                                                     // Loop start time (uS)

int     Gyro_x,     Gyro_y,     Gyro_z;
long    Gyro_x_cal, Gyro_y_cal, Gyro_z_cal;
float   Gyro_pitch, Gyro_roll, Gyro_yaw;
float   Gyro_pitch_output, Gyro_roll_output;

// ----- Accelerometer
long    Accel_x,      Accel_y,      Accel_z,    Accel_total_vector;
float   Accel_pitch,  Accel_roll;

// ----- Compass heading
/*
  The magnetic declination for Lower Hutt, New Zealand is +22.5833 degrees
  Obtain your magnetic declination from http://www.magnetic-declination.com/
  Uncomment the declination code within the main loop() if you want True North.
*/
float   Declination = +22.5833;                                             //  Degrees ... replace this declination with yours
int     Heading;

int     Mag_x,                Mag_y,                Mag_z;                  // Raw magnetometer readings
float   Mag_x_dampened,       Mag_y_dampened,       Mag_z_dampened;
float   Mag_x_hor, Mag_y_hor;
float   Mag_pitch, Mag_roll;

// ----- Record compass offsets, scale factors, & ASA values
/*
   These values seldom change ... an occasional check is sufficient
   (1) Open your Arduino "Serial Monitor
   (2) Set "Record_data=true;" then upload & run program.
   (3) Replace the values below with the values that appear on the Serial Monitor.
   (4) Set "Record_data = false;" then upload & rerun program.
*/
bool    Record_data = false;
int     Mag_x_offset = 315.89,      Mag_y_offset = -12.82,     Mag_z_offset = 17.59;   // Hard-iron offsets
float   Mag_x_scale = 0.75,     Mag_y_scale = 0.82,     Mag_z_scale = 2.25;    // Soft-iron scale factors
float   ASAX = 1.21094,            ASAY = 1.21484,            ASAZ = 1.16797;           // (A)sahi (S)ensitivity (A)djustment fuse ROM values.

// ----- Flags
bool Gyro_synchronised = false;
bool Flag = false;

// ----- Debug
bool Switch = false;                     
long Loop_start_time;
long Debug_start_time;

// IT'S PENDING TO SET THE RIGHT COMPENSATING VALUES FOR SENSITIVITY ADJUSTMENT FROM BMX160
// (A)sahi (S)ensitivity (A)djustment fuse ROM values.
//float ASAX = 1.21;
//float ASAY = 1.21; 
//float ASAZ = 1.17;

void setup(void)
{
  Serial.begin(115200);     // FOR THE SERIAL MONITOR

  // Wait for the Serial Monitor
  while (!Serial) {
    delay(1);
  }

  // Initialise the sensor
  dpEng.begin(BMX160_ACCELRANGE_4G, GYRO_RANGE_250DPS);

  // ----- Calibrate the magnetometer
  /*
     Calibrate only needs to be done occasionally.
     Enter the magnetometer values into the "header"
     then set "Record_data = false".
  */
  if (Record_data == true)
  {
    calibrate_magnetometer();
  }
  //calibrate_gyro();

  Debug_start_time = micros();                          // Controls data display rate to Serial Monitor
  Loop_start_time = micros();                           // Controls the Gyro refresh rate
}

void loop()
{
  read_bmx160_data();
  // ----- Adjust for offsets
  Gyro_x -= Gyro_x_cal;                                             // Subtract the offset from the raw gyro_x value
  Gyro_y -= Gyro_y_cal;                                             // Subtract the offset from the raw gyro_y value
  Gyro_z -= Gyro_z_cal;                                             // Subtract the offset from the raw gyro_z value
  
  // ----- Calculate travelled angles
  /*
    ---------------------------
    Adjust Gyro_xyz signs for:
    ---------------------------
    Pitch (Nose - up) = +ve reading
    Roll (Right - wing down) = +ve reading
    Yaw (Clock - wise rotation)  = +ve reading
  */
  Gyro_pitch += -Gyro_y * Sensor_to_deg;                            // Integrate the raw Gyro_y readings
  Gyro_roll += Gyro_x * Sensor_to_deg;                              // Integrate the raw Gyro_x readings
  Gyro_yaw += -Gyro_z * Sensor_to_deg;                              // Integrate the raw Gyro_x readings

  // ----- Compensate pitch and roll for gyro yaw
  Gyro_pitch += Gyro_roll * sin(Gyro_z * Sensor_to_rad);            // Transfer the roll angle to the pitch angle if the Z-axis has yawed
  Gyro_roll -= Gyro_pitch * sin(Gyro_z * Sensor_to_rad);            // Transfer the pitch angle to the roll angle if the Z-axis has yawed

  // ----- Accelerometer angle calculations
  Accel_total_vector = sqrt((Accel_x * Accel_x) + (Accel_y * Accel_y) + (Accel_z * Accel_z));   // Calculate the total (3D) vector
  Accel_pitch = asin((float)Accel_x / Accel_total_vector) * RAD_TO_DEG;                         //Calculate the pitch angle
  Accel_roll = asin((float)Accel_y / Accel_total_vector) * RAD_TO_DEG;                         //Calculate the roll angle

  // ----- Zero any residual accelerometer readings
  /*
     Place the accelerometer on a level surface
     Adjust the following two values until the pitch and roll readings are zero
  */
  //Accel_pitch -= -0.2f;                                             //Accelerometer calibration value for pitch
  //Accel_roll -= 1.1f;                                               //Accelerometer calibration value for roll

  // ----- Correct for any gyro drift
  if (Gyro_synchronised)
  {
    // ----- Gyro & accel have been synchronised
    Gyro_pitch = Gyro_pitch * 0.9996 + Accel_pitch * 0.0004;        //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    Gyro_roll = Gyro_roll * 0.9996 + Accel_roll * 0.0004;           //Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else
  {
    // ----- Synchronise gyro & accel
    Gyro_pitch = Accel_pitch;                                       //Set the gyro pitch angle equal to the accelerometer pitch angle
    Gyro_roll = Accel_roll;                                         //Set the gyro roll angle equal to the accelerometer roll angle
    Gyro_synchronised = true;                                             //Set the IMU started flag
  }

  // ----- Dampen the pitch and roll angles
  Gyro_pitch_output = Gyro_pitch_output * 0.9 + Gyro_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  Gyro_roll_output = Gyro_roll_output * 0.9 + Gyro_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value

    ////////////////////////////////////////////
  //        MAGNETOMETER CALCULATIONS       //
  ////////////////////////////////////////////
  /*
     --------------------------------
     Instructions for first time use
     --------------------------------
     Calibrate the compass for Hard-iron and Soft-iron
     distortion by temporarily setting the header to read
     bool    Record_data = true;

     Turn on your Serial Monitor before uploading the code.

     Slowly tumble the compass in all directions until a
     set of readings appears in the Serial Monitor.

     Copy these values into the appropriate header locations.

     Edit the header to read
     bool    Record_data = false;

     Upload the above code changes to your Arduino.

     This step only needs to be done occasionally as the
     values are reasonably stable.
  */

  // ----- Read the magnetometer
  read_magnetometer();

  // ----- Fix the pitch, roll, & signs
  /*
     MPU-9250 gyro and AK8963 magnetometer XY axes are orientated 90 degrees to each other
     which means that Mag_pitch equates to the Gyro_roll and Mag_roll equates to the Gryro_pitch

     The MPU-9520 and AK8963 Z axes point in opposite directions
     which means that the sign for Mag_pitch must be negative to compensate.
  */
  Mag_pitch = abs(Accel_roll) * DEG_TO_RAD;
  Mag_roll = abs(Accel_pitch) * DEG_TO_RAD;

  // ----- Apply the standard tilt formulas
  Mag_y_hor = Mag_x * cos(Mag_pitch) + Mag_y * sin(Mag_roll) * sin(Mag_pitch) - Mag_z * cos(Mag_roll) * sin(Mag_pitch);
  Mag_x_hor = Mag_y * cos(Mag_roll) + Mag_z * sin(Mag_roll);

  // ----- Disable tilt stabization if switch closed
  /*if (!Switch)
  {
    // ---- Test equations
    Mag_x_hor = Mag_x;
    Mag_y_hor = Mag_y;
  }*/
  
  // ----- Dampen any data fluctuations
  Mag_x_dampened = Mag_x_dampened * 0.9 + Mag_x_hor * 0.1;
  Mag_y_dampened = Mag_y_dampened * 0.9 + Mag_y_hor * 0.1;

  // ----- Calculate the heading
  Heading = atan2(Mag_y_dampened, Mag_x_dampened) * RAD_TO_DEG;  // Magnetic North

  // ----- Correct for True North
  //    Heading += Declination;                                   // Geographic North

  // ----- Allow for under/overflow
  if (Heading < 0) Heading += 360;
  if (Heading >= 360) Heading -= 360;

  Serial.print("Mag_x_hor:");Serial.print(Mag_x_hor);
  Serial.print(" Mag_y_hor:");Serial.print(Mag_y_hor);
  Serial.print(" Mag_z:");Serial.print(Mag_z);
  Serial.print(" Heading:");Serial.print(Heading);
  Serial.print(" Accel_pitch:");Serial.print(abs(Accel_pitch));
  Serial.print(" Accel_roll:");Serial.println(abs(Accel_roll));
  //Serial.print(" Gyro_pitch:");Serial.print(Gyro_pitch);
  //Serial.print(" Gyro_roll:");Serial.println(Gyro_roll);
  while ((micros() - Loop_start_time) < 8400);
  Loop_start_time = micros();
}

// -------------------------------
//  Calibrate magnetometer
// -------------------------------
void calibrate_magnetometer()
{
  // ----- Locals
  int mag_x, mag_y, mag_z;

  int mag_x_min =  32767;                                         // Raw data extremes
  int mag_y_min =  32767;
  int mag_z_min =  32767;
  int mag_x_max = -32768;
  int mag_y_max = -32768;
  int mag_z_max = -32768;

  float chord_x,  chord_y,  chord_z;                              // Used for calculating scale factors
  float chord_average;

  // ----- Record min/max XYZ compass readings
  for (int counter = 0; counter < 1000 ; counter ++)             // Run this code 16000 times
  {
    Loop_start = micros();
    // ----- Locals
    int mag_x, mag_y, mag_z;
    sensors_event_t aevent, gevent, mevent;

    /* Read raw data for x, y and z axes and include factory 
    compensation values */
    dpEng.getEvent(&aevent, &gevent, &mevent);

    mag_x = mevent.magnetic.x*ASAX;     // Combine LSB,MSB X-axis, apply ASA corrections
    mag_y = mevent.magnetic.y*ASAY;     // Combine LSB,MSB X-axis, apply ASA corrections
    mag_z = mevent.magnetic.z*ASAZ;     // Combine LSB,MSB X-axis, apply ASA correction

    // ----- Find max/min xyz values
    mag_x_min = min(mag_x, mag_x_min);
    mag_x_max = max(mag_x, mag_x_max);
    mag_y_min = min(mag_y, mag_y_min);
    mag_y_max = max(mag_y, mag_y_max);
    mag_z_min = min(mag_z, mag_z_min);
    mag_z_max = max(mag_z, mag_z_max);
    delay(4);                                                    // Time interval between magnetometer readings
    Serial.print("Calibrating until: ");Serial.print(1000 - counter);  
    Serial.print(" max_x: ");Serial.print( mag_x_max);
    Serial.print(" min_x: ");Serial.print( mag_x_min);
    Serial.print(" max_y: ");Serial.print( mag_y_max);
    Serial.print(" min_y: ");Serial.print( mag_y_min);
    Serial.print(" max_z: ");Serial.print( mag_z_max);
    Serial.print(" min_z: ");Serial.println( mag_z_min);
    delay(4);
  }

  // ----- Calculate hard-iron offsets
  Mag_x_offset = (mag_x_max + mag_x_min) / 2;                     // Get average magnetic bias in counts
  Mag_y_offset = (mag_y_max + mag_y_min) / 2;
  Mag_z_offset = (mag_z_max + mag_z_min) / 2;

  // ----- Calculate soft-iron scale factors
  chord_x = ((float)(mag_x_max - mag_x_min)) / 2;                 // Get average max chord length in counts
  chord_y = ((float)(mag_y_max - mag_y_min)) / 2;
  chord_z = ((float)(mag_z_max - mag_z_min)) / 2;

  chord_average = (chord_x + chord_y + chord_z) / 3;              // Calculate average chord length

  Mag_x_scale = chord_average / chord_x;                          // Calculate X scale factor
  Mag_y_scale = chord_average / chord_y;                          // Calculate Y scale factor
  Mag_z_scale = chord_average / chord_z;                          // Calculate Z scale factor

  // ----- Record magnetometer offsets
  /*
     When active this feature sends the magnetometer data
     to the Serial Monitor then halts the program.
  */
  if (Record_data == true)
  {
    // ----- Display data extremes
    Serial.print("XYZ Max/Min: ");
    Serial.print(mag_x_min); Serial.print("\t");
    Serial.print(mag_x_max); Serial.print("\t");
    Serial.print(mag_y_min); Serial.print("\t");
    Serial.print(mag_y_max); Serial.print("\t");
    Serial.print(mag_z_min); Serial.print("\t");
    Serial.println(mag_z_max);
    Serial.println("");

    // ----- Display hard-iron offsets
    Serial.print("Hard-iron: ");
    Serial.print(Mag_x_offset); Serial.print("\t");
    Serial.print(Mag_y_offset); Serial.print("\t");
    Serial.println(Mag_z_offset);
    Serial.println("");

    // ----- Display soft-iron scale factors
    Serial.print("Soft-iron: ");
    Serial.print(Mag_x_scale); Serial.print("\t");
    Serial.print(Mag_y_scale); Serial.print("\t");
    Serial.println(Mag_z_scale);
    Serial.println("");

    // ----- Display fuse ROM values
    Serial.print("ASA: ");
    Serial.print(ASAX); Serial.print("\t");
    Serial.print(ASAY); Serial.print("\t");
    Serial.println(ASAZ);

    // ----- Halt program
    while (true);                                       // Wheelspin ... program halt
  }
}

void read_magnetometer()
{
  // ----- Locals
  int mag_x, mag_y, mag_z;
  sensors_event_t aevent, gevent, mevent;

  /* Read raw data for x, y and z axes and include factory 
    compensation values */
  dpEng.getEvent(&aevent, &gevent, &mevent);

  mag_x = mevent.magnetic.x*ASAX;     // Combine LSB,MSB X-axis, apply ASA corrections
  mag_y = mevent.magnetic.y*ASAY;     // Combine LSB,MSB X-axis, apply ASA corrections
  mag_z = mevent.magnetic.z*ASAZ;     // Combine LSB,MSB X-axis, apply ASA correction
  
  Mag_x = (mag_x - Mag_x_offset);// * Mag_x_scale;
  Mag_y = (mag_y - Mag_y_offset);// * Mag_y_scale;
  Mag_z = (mag_z - Mag_z_offset);// * Mag_z_scale;
}

// -----------------------------------
//  Calibrate gyro
// -----------------------------------
void calibrate_gyro()
{
  // ----- Calibrate gyro
  for (int counter = 0; counter < 2000 ; counter ++)    //Run this code 2000 times
  {
    Loop_start = micros();
    read_bmx160_data();                               //Read the raw acc and gyro data from the MPU-6050
    Gyro_x_cal += Gyro_x;                               //Add the gyro x-axis offset to the gyro_x_cal variable
    Gyro_y_cal += Gyro_y;                               //Add the gyro y-axis offset to the gyro_y_cal variable
    Gyro_z_cal += Gyro_z;                               //Add the gyro z-axis offset to the gyro_z_cal variable
    while (micros() - Loop_start < Loop_time);           // Wait until "Loop_time" microseconds have elapsed
  }
  Gyro_x_cal /= 2000;                                   //Divide the gyro_x_cal variable by 2000 to get the average offset
  Gyro_y_cal /= 2000;                                   //Divide the gyro_y_cal variable by 2000 to get the average offset
  Gyro_z_cal /= 2000;                                   //Divide the gyro_z_cal variable by 2000 to get the average offset
}

void read_bmx160_data()
{
  sensors_event_t aevent, gevent, mevent;

  /* Read raw data for x, y and z axes and include factory 
    compensation values */
  dpEng.getEvent(&aevent, &gevent, &mevent);
  Accel_x = aevent.acceleration.x;
  Accel_y = aevent.acceleration.y;
  Accel_z = aevent.acceleration.z;
  Gyro_x = aevent.gyro.x;
  Gyro_y = aevent.gyro.y;
  Gyro_z = aevent.gyro.z;
  //mx = mevent.magnetic.x;
  //my = mevent.magnetic.y;
  //mz = mevent.magnetic.z;
}
