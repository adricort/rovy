#include "Adafruit_MLX90393.h"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// For magnetometer MXL90393 ----------------------
Adafruit_MLX90393 sensor = Adafruit_MLX90393(); // Object creation
#define MLX90393_CS 10    // CS pin definition

const int buzzerPin = 25;  // 16 corresponds to GPIO16
const int rhPin = 26;  // 16 corresponds to GPIO16
const int led_red = 33; // GPIO 33 connected to red LED on PCB
const int led_yellow = 17;
const int led_green = 16;

// setting PWM properties
const int rhFreq = 4000;
const int buzzerFreq = 2000;
const int rhChannel = 0;
const int buzzerChannel = 1;
const int resolution = 8;

int magx, magy, magz;
int rw_pwm = 0;
int command = 0;

void setup(){
  Serial.begin(115200);   // Initialization of the serial port at a set baudrate
  // configure PWM functionalitites
  ledcSetup(rhChannel, rhFreq, resolution);
  ledcSetup(buzzerChannel, buzzerFreq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(buzzerPin, buzzerChannel);
  ledcAttachPin(rhPin, rhChannel);

  pinMode(led_red,OUTPUT);
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);

  if (! sensor.begin_I2C()) {          // hardware I2C mode, can pass in address & alt Wire
  //if (! sensor.begin_SPI(MLX90393_CS)) {  // hardware SPI mode
    Serial.println("No sensor found ... check your wiring?");
    while (1) { delay(10); }
  }
  Serial.println("Found a MLX90393 sensor");

  sensor.setGain(MLX90393_GAIN_2_5X);
  // You can check the gain too
  Serial.print("Gain set to: ");
  switch (sensor.getGain()) {
    case MLX90393_GAIN_1X: Serial.println("1 x"); break;
    case MLX90393_GAIN_1_33X: Serial.println("1.33 x"); break;
    case MLX90393_GAIN_1_67X: Serial.println("1.67 x"); break;
    case MLX90393_GAIN_2X: Serial.println("2 x"); break;
    case MLX90393_GAIN_2_5X: Serial.println("2.5 x"); break;
    case MLX90393_GAIN_3X: Serial.println("3 x"); break;
    case MLX90393_GAIN_4X: Serial.println("4 x"); break;
    case MLX90393_GAIN_5X: Serial.println("5 x"); break;
  }

  // Set resolution, per axis
  sensor.setResolution(MLX90393_X, MLX90393_RES_19);
  sensor.setResolution(MLX90393_Y, MLX90393_RES_19);
  sensor.setResolution(MLX90393_Z, MLX90393_RES_16);

  // Set oversampling
  sensor.setOversampling(MLX90393_OSR_2);

  // Set digital filtering
  sensor.setFilter(MLX90393_FILTER_6);

  SerialBT.begin("ESP32test"); //Bluetooth device name

  digitalWrite(led_red,HIGH);
  while(SerialBT.available()==0){}
  command = SerialBT.parseInt();
  //command = 1;
}
 
void loop(){
  if(command == 1)
  {
    digitalWrite(led_red,LOW);
    sensors_event_t event;
    sensor.getEvent(&event);
  
    int magx = abs(event.magnetic.x);
    int magy = abs(event.magnetic.y);
    int magz = abs(event.magnetic.z);
  
    // HERE <----------- USER INPUT
    // Inputs from the user when calibrating.
    int x_max=36,x_min=3,x_side=15,y_max=69,y_min=33,y_side=40;
    int k = 17;
  
    if(magy > y_side)
      ledcWrite(buzzerChannel, 255);
    else if(magy < y_side)
      ledcWrite(buzzerChannel, 0);

    if(abs(magx-x_min) < 10 && abs(magy-y_side) < 10)
    {
      digitalWrite(led_green,HIGH);
      digitalWrite(led_yellow,LOW);
      digitalWrite(led_red,LOW);
      //k = 2;
      //rw_pwm = (magy-y_side)*k;
    }
    else
    {
      //k = 17;
      digitalWrite(led_green,LOW);
      digitalWrite(led_yellow,HIGH);
      digitalWrite(led_red,LOW);
      //rw_pwm = (magx)*k;
    }

    rw_pwm = (magx)*k;
    
    if(rw_pwm > 150)
      rw_pwm = 150;
    else if(rw_pwm < 0)
      rw_pwm = 0;
    if(magx == 3)
      rw_pwm = 3;
    
    ledcWrite(rhChannel, rw_pwm);
    
    // Display the results
    /*Serial.print("X: "); Serial.print(magx);
    Serial.print(" Y: "); Serial.print(magy);
    Serial.print(" Z: "); Serial.print(magz);
    Serial.print(" rw_pwm: "); Serial.print(rw_pwm);
    Serial.println();*/
  }
}
