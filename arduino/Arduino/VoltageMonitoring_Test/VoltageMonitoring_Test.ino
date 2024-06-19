#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>

Adafruit_INA219 ina219_A(0x40); //INA1
Adafruit_INA219 ina219_B(0x41); //INA2

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
float busvoltage_A = 0;
float current_A = 0;
float busvoltage_B = 0;
float current_B = 0;

void setup(void) 
{
  Serial.begin(115200);
  ina219_A.begin();  // Initialize first board (default address 0x40)
  ina219_B.begin();  // Initialize second board with the address 0x41
  //display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  Serial.println("Measuring voltage and current with INA219 ...");
}

void loop(void) 
{
  busvoltage_A = 0;
  current_A = 0;

  busvoltage_B = 0;
  current_B = 0;

  busvoltage_A = ina219_A.getBusVoltage_V();
  current_A = ina219_A.getCurrent_mA();

  busvoltage_B = ina219_B.getBusVoltage_V();
  current_B = ina219_B.getCurrent_mA();

  //display.clearDisplay();
  Voltage_A(busvoltage_A);
  Voltage_B(busvoltage_B);
 
  //delay(500);
}

void Voltage_A(float busvoltage_A) {
  Serial.println(busvoltage_A);
  //Serial.println("hello");
}

void Voltage_B(float busvoltage_B) {
  Serial.println(busvoltage_B);

}
