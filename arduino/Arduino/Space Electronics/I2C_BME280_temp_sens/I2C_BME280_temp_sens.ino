#include <Wire.h>

const int BME280_I2C = 0x77;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(BME280_I2C);
  Wire.write(0xD0); // Set register to read (from datasheet)
  if(!Wire.endTransmission())
  {
    Wire.requestFrom(BME280_I2C,1);
    byte id = Wire.read();
    Serial.print("0x");
    Serial.println(id, HEX);
  }
}

void loop() {
}
