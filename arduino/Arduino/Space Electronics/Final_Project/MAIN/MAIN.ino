/* Program developed by Adrian Ricardez Ortigosa, with SN. 452150
 *  Subject: Space Electronics, Lecturer: Cem Avsar and Sebastian Lange
 *  Final Project MAIN program
 *  This program meets all the final project requirements stablished in the pdf of the assignment
 *  Everhything worked with the measured magnetic field of Charlottenburg, without any disturbance in the near surroundings
 *  and with the WiFi of a student residence building. The RSSI and the uT depend totally on this, but the
 *  magnetic field sensor function can be calibrated beforehand.
  USER INPUTS: Main.ino 58, 59, and Mode3_CompassMode 13
 */

// ~~~~~~~~~~~ DEVICE LIBRARY DEFINITION ~~~~~~~~~~~~~~
#include <ESP32Servo.h>         // For Servo with ESP32
#include <WiFi.h>               // For WiFi 
#include <WiFiMulti.h>          // For Multi WiFi
#include <SPI.h>                // For several devices considered
#include <Wire.h>               // For several devices considered
#include <Adafruit_GFX.h>       // For OLED
#include <Adafruit_SSD1306.h>   // For OLED
#include <Adafruit_LSM6DS33.h>  // For the IMU LSM6DS33
#include "Adafruit_MLX90393.h"  // For the magnetic-field sensor MLX90393
#include <Adafruit_Sensor.h>    // For the humidity sensor BME280
#include <Adafruit_BME280.h>    // For the humidity sensor BME280

// ========================= CONFIGURATIONS ================================
// For OLED SSD1306 -------------------------------
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);  // Object creation

// For magnetometer MXL90393 ----------------------
Adafruit_MLX90393 sensor = Adafruit_MLX90393(); // Object creation
#define MLX90393_CS 10    // CS pin definition

// For IMU LMS6DS33 -------------------------------
// For SPI mode, we need a CS pin
#define LSM_CS 5
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LSM_SCK 18
#define LSM_MISO 19
#define LSM_MOSI 23
Adafruit_LSM6DS33 lsm6ds33;   // Object creation

// For humidity sensor BME280 ---------------------
// Pin definition for protocols
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)  // Definition of sea-level pressure in hPa
Adafruit_BME280 bme;      // I2C object creation

// For Servo --------------------------------------
Servo myservo;        // Servo object creation

// For WiFi ---------------------------------------
WiFiMulti wifiMulti;  // WiFi object creation
#define MAX_SRV_CLIENTS 1 // n. of clients that should be able to telnet to this ESP32
const char* ssid = "LAPTOP-O4J1HNIM 9037";    // Available SSID <----------- USER INPUT
const char* password = "9Z2i0@67";            // SSID Password  <----------- USER INPUT 
WiFiServer server(23);                        // Server configuration
WiFiClient serverClients[MAX_SRV_CLIENTS];    // Setting the previous max-srv-client variable

// For timer interrupt ----------------------------
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// ========================= GLOBAL VARIABLES ================================ 
float gyrox, gyroy, gyroz, accelx, accely, accelz, temp, hum, pres, rssi = 0; // Displayed variables
int magx, magy, magz, sang, ldr;  // Displayed variables
int Mode = 0;                     // The one for the switch case
const int threshold = 30;         // For the touch buttons
int servo_angle_north = 90;       // Initialization for Mode 3
int touch_previousUP = 0,touch_previousDOWN = 0;  // For the touch buttons filter
boolean timer_flag = false;                       // For the timer interrupt every 2 seconds  

// ========================= PIN DEFINITION ================================ 
const int LDR_pin = 35;       // LDR pin
const int LEDgreen_pin = 16;  // Green LED pin
const int LEDyellow_pin = 17; // Yellow LED pin
const int LEDred_pin = 33;    // Red LED pin
const int Servo_pin = 26;     // Servo pin

// ------------- Touch read function (more effective than touchInterrupt) -----------------
void TouchRead(int CHANNEL)
{
  int touchUP = touchRead(T0);    // Reading the touch button UP
  int touchDOWN = touchRead(T3);  // Reading the touch button DOWN
  delayMicroseconds(10);          // Giving time to avoid reading the same previous value
  int touchUP2 = touchRead(T0);   // Reading the touch button (second time, for the filter)
  int touchDOWN2 = touchRead(T3); // Reading the touch button (second time, for the filter)
  
  // digitalizing values for better response:
  if(touchUP < threshold)         // If the first touch read it is below the defined threshold
    touchUP = 1;                  // digitalizes to 1
  else if(touchDOWN < threshold)  // If the first touch read it is below the defined threshold
    touchDOWN = 1;                // digitalizes to 1
  else                            // If none of the previous cases
  {
    touchUP = 0;                  // digitalizes to 0
    touchDOWN = 0;                // digitalizes to 0
  }
  if(touchUP2 < threshold)        // If the second touch read it is below the defined threshold
    touchUP2 = 1;                 // digitalizes to 1
  else if(touchDOWN2 < threshold) // If the second touch read it is below the defined threshold
    touchDOWN2 = 1;               // digitalizes to 1
  else                            // If none of the previous cases
  {
    touchUP2= 0;                  // digitalizes to 0
    touchDOWN2 = 0;               // digitalizes to 0
  }

  // Debouncing + filter UP -------------------------------------------------------
  if(touchUP == 1 && touchUP2 == 1 && touch_previousUP == 0)    // 3-value filter push
  {
    Mode++;       // Increase the mode value once
    if(Mode > 5)  // Delimitation of modes
      Mode = 5;
    for(int i = 0; i < Mode; i++) // Ring the buzzer depending on the mode
    {
      ledcWriteTone(CHANNEL, 1500);   // turns on the buzzer with channel 5 at 1500 Hz
      delay(50);                      // waits 50 miliseconds
      ledcWriteTone(CHANNEL, 0);      // turns off the buzzer
      delay(50);                      // waits 50 miliseconds
    }
    touch_previousUP = 1;     // Update the last value for the debouncing correction
    display.clearDisplay();   // Clear the display
    display.display();        // Update the display
  }
  else if(touchUP == 0 && touchUP2 == 0 && touch_previousUP == 1) // 3-value filter release
    touch_previousUP = 0;     // Go to the initial state
  // End debouncing + filter UP -----------------------------------------------------

  // Debouncing + filter DOWN -------------------------------------------------------
  if(touchDOWN == 1 && touchDOWN2 == 1 && touch_previousDOWN == 0)    // 3-value filter push
  {
    Mode--;         // Decrease the mode value once
    if(Mode < 0)    // Delimitation of modes
      Mode = 0;
    for(int i = 0; i < Mode; i++)   // Ring the buzzer depending on the mode
    {
      ledcWriteTone(CHANNEL, 500);  // turns on the buzzer with channel 5 at 500 Hz
      delay(50);                    // waits 50 miliseconds
      ledcWriteTone(CHANNEL, 0);    // turns off the buzzer
      delay(50);                    // waits 50 miliseconds
    }
    touch_previousDOWN = 1;   // Update the last value for the debouncing correction
    display.clearDisplay();   // Clear the display
    display.display();        // Update the display
  }
  else if(touchDOWN == 0 && touchDOWN2 == 0 && touch_previousDOWN == 1) // 3-value filter release
    touch_previousDOWN = 0;   // Go to the initial state
  // End debouncing + filter DOWN ---------------------------------------------------
}

// ------------- Timer interrupt function for the Houskeeping data sending -----------------
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);    // enters and assigns the timerMux
  timer_flag = true;                    // turns on the flag to meet the 2 seconds conditional
  portEXIT_CRITICAL_ISR(&timerMux);     // exits and assigns the timerMux
}

// ~~~~~~~ HEADER FILES DEFINITION (modes and functions) ~~~~~~~~
#include "Houskeeping_data_WiFi.h"  // Transmits the houskeeping sensor data through WiFi in all modes and receives commands
#include "sensor_getvalues.h"       // Gets the sensor values
#include "Mode0_StandbyMode.h"      // Waits for instructions from touch buttons or WiFi commands
#include "Mode1_ServoLDRMode.h"     // Moves the servo depending on the light intensity of the LDR
#include "Mode2_MelodyMode.h"       // Rings a beacon melody of more than 10 tones within 3 seconds, every 10 seconds
#include "Mode3_CompassMode.h"      // Within a range, the servo points to the north based on magnetic measurements
#include "Mode4_RSSILEDMode.h"      // Lights the 3 LEDs depending on the rssi WiFi signal
#include "Mode5_EquilibriumMode.h"  // Like a drone, this has a P-controller for SDC equilibrium

// ================================================================================
// =============================== VOID SETUP =====================================
// ================================================================================
void setup() {
  Serial.begin(115200);   // Initialization of the serial port at a set baudrate
  while(!Serial){delay(10);}    // Wait for serial on USB platforms (useful for several components)
  #define PIN_TONE 25   // For the buzzer (needed to be defined mandatorily inside of the setup)
  #define CHANNEL 5     // For the ledc function (pwm) (needed to be defined mandatorily inside of the setup)

  // PinModes:
  pinMode(PIN_TONE,OUTPUT); // Configuring the buzzer pin as an output
  pinMode(LDR_pin,INPUT);   // Configuring the LDR pin as an input (maybe not needed)
  pinMode(LEDgreen_pin,OUTPUT);   // Configuring the green LED pin as an output
  pinMode(LEDyellow_pin,OUTPUT);  // Configuring the yellow LED pin as an output
  pinMode(LEDred_pin,OUTPUT);     // Configuring the red LED pin as an output
  myservo.attach(Servo_pin);    // Attaching the servo pin to the attach servo function
  
  // MLX90393 configuration:
  if (! sensor.begin_I2C()) {               // hardware I2C mode, can pass in address & alt Wire
    Serial.println("No sensor found ... check your wiring?");
    while (1) { delay(10); }}
  sensor.setGain(MLX90393_GAIN_2_5X);   // Setting the gain
  sensor.setResolution(MLX90393_X, MLX90393_RES_19);  // Resolution x axis
  sensor.setResolution(MLX90393_Y, MLX90393_RES_19);  // Resolution y axis
  sensor.setResolution(MLX90393_Z, MLX90393_RES_16);  // Resolution z axis
  sensor.setOversampling(MLX90393_OSR_2); // Set oversampling
  sensor.setFilter(MLX90393_FILTER_6);    // Set digital filtering

  // Buzzer configuration (PWM):
  ledcSetup(CHANNEL, 5000, 8);      // Channel, frequency, and resolution
  ledcAttachPin(PIN_TONE, CHANNEL); // Attachment pin, and channel
  ledcWrite(CHANNEL, 0);            // Duty Cycle inizialized to 0

  // OLED configuration:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.setCursor(0,0);   // Go to the beginning of the matrix
  delay(1000);              // Give a bit of time
  display.clearDisplay();   // Clear the buffer
  display.display();        // Display the buffer
  display.setTextSize(1);   // text display tests
  display.setTextColor(SSD1306_WHITE);  // Set the text color to white
  display.print("Waiting for WiFi connection ... ");  // Initial state
  display.display();        // Display the buffer

  // LMS6DS33 Configuration:
  if (!lsm6ds33.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    Serial.println("Failed to find LSM6DS33 chip");
    while (1) {delay(10);}}
  lsm6ds33.configInt1(false, false, true);  // accelerometer DRDY on INT1
  lsm6ds33.configInt2(false, true, false);  // gyro DRDY on INT2

  // WiFi Configuration:
  // adding the specified ssid and password enterder by the programmer
  wifiMulti.addAP(ssid, password);
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  // Connecting to the WiFi withing 10 seconds
  for (int loops = 10; loops > 0; loops--) {
    if (wifiMulti.run() == WL_CONNECTED) {
      display.clearDisplay();       // Clear the display
      display.display();            // Update the display
      display.setCursor(0,0);       // Go to the beginning of the matrix
      display.print("WiFi connected!"); // Print message
      display.display();            // Update the display
      Serial.print("WiFi connected ");  // Print message on the Serial Monitor
      Serial.print("IP address: ");     // Print message on the Serial Monitor
      Serial.println(WiFi.localIP());   // Print IP on the Serial Monitor
      delay(1000);
      break;
    }
    else {    // if not, print the loops
      Serial.println(loops);
      delay(1000);
    }
  }
  // If it doesn't connect
  if (wifiMulti.run() != WL_CONNECTED) {
    display.clearDisplay();       // Clear the display
    display.display();            // Update the display
    display.setCursor(0,0);       // Go to the beginning of the matrix
    display.print("WiFi connect failed"); // Print message on the matrix
    display.display();            // Update the display
    Serial.println("WiFi connect failed");  // Print message on the Serial Monitor
    delay(1000);                  // Wait for 1 second
    display.clearDisplay();       // Clear the display
    display.display();            // Update the display
    ESP.restart();                // Restart the entire board
  }
  server.begin();     // One it has reached the WiFi connection, it starts the server
  server.setNoDelay(true);  // Sets no delay

  // Instructions for connectivity through telnet from the server
  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");

  // BME280 Configuration:
  unsigned status;
  // default settings
  status = bme.begin();
  // Checks the status of the device in case that it is not found
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }

  // Timer Interrupt Configuration:
  timer = timerBegin(0, 80, true);              // Beginning (# of timer, prescaler->divides to get 1 MHz, counter up or down)
  timerAttachInterrupt(timer, &onTimer, true);  // Attachment (object, declared function, edge of type)
  timerAlarmWrite(timer, 2000000, true);        // Counter value (object, microseconds, automatic reload)
  timerAlarmEnable(timer);      // Enabling the timer
  display.clearDisplay();       // Clear the display
  display.display();            // Update the display
}

// ================================================================================
// =============================== VOID LOOP ======================================
// ================================================================================
void loop(){
  // Calling all the functions to get the sensor values (except the rssi, that is inside of Houskeeping_data_WiFi())
  LSM6DS33_getvalues(gyrox, gyroy, gyroz, accelx, accely, accelz);  // Sensor values of the IMU
  MLX90393_getvalues(magx, magy, magz);                             // Sensor values of the magnetometer
  Servo_getvalues(sang);                                            // Encoder values of the Servo
  BME280_getvalues(temp, hum, pres);                                // Sensor values of the humidity sensor
  LDR_getvalues(ldr);                                               // Sensor values of the LDR
  Houskeeping_data_WiFi();                                          // Sensor values of the rssi and Houskeeping-data sending
  
  // Enters each mode:
  /*depending on the global variable "Mode" that changes with the touch buttons and the server terminal commands*/
  switch(Mode)
  {
    case 0:                                       // If the global variable is == 0
      Mode0_StandbyMode(CHANNEL);                 // Enters Mode 0
      break;                                      // breaks the switch(case) structure
    case 1:                                       // If the global variable is == 1
      Mode1_ServoLDRMode(ldr, CHANNEL);           // Enters Mode 1 
      break;                                      // breaks the switch(case) structure
    case 2:                                       // If the global variable is == 2
      Mode2_MelodyMode(CHANNEL);                  // Enters Mode 2
      break;                                      // breaks the switch(case) structure
    case 3:                                       // If the global variable is == 3
      Mode3_CompassMode(magx,magy,magz,CHANNEL);  // Enters Mode 3
      break;                                      // breaks the switch(case) structure
    case 4:                                       // If the global variable is == 4
      Mode4_RSSILEDMode(rssi,CHANNEL);            // Enters Mode 4
      break;                                      // breaks the switch(case) structure
    case 5:                                       // If the global variable is == 5
      Mode5_EquilibriumMode(CHANNEL,accelx);      // Enters Mode 5
      break;                                      // breaks the switch(case) structure
  }
}
