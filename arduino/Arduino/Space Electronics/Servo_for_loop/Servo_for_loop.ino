// Include the ESP32 Arduino Servo Library instead of the original Arduino Servo Library
#include <ESP32Servo.h> 

Servo myservo;  // create servo object to control a servo
int servoPin = 26;      // GPIO pin used to connect the servo control (digital out)

void setup()
{
  Serial.begin(9600);
  myservo.attach(servoPin);
}

void loop() {
  for(int i = 0; i < 180; i++)
  {
    myservo.write(i);                  // set the servo position according to the scaled value
    Serial.println(90-myservo.read());
    delay(15);                          // wait for the servo to get there
  }
  for(int i = 180; i > 0; i--)
  {
    myservo.write(i);                  // set the servo position according to the scaled value
    Serial.println(90-myservo.read());
    delay(15);                          // wait for the servo to get there
  }
}
