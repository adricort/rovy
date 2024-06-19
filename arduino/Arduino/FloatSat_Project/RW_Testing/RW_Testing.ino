// the number of the LED pin
//const int buzzerPin = 25;  // 16 corresponds to GPIO16
const int buzzerPin = 25;  // 16 corresponds to GPIO16
const int rhPin = 26;  // 16 corresponds to GPIO16

// setting PWM properties
const int rhFreq = 4000;
const int buzzerFreq = 2000;
const int rhChannel = 0;
const int buzzerChannel = 1;
const int resolution = 8;
 
void setup(){
  // configure LED PWM functionalitites
  ledcSetup(rhChannel, rhFreq, resolution);
  ledcSetup(buzzerChannel, buzzerFreq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(buzzerPin, buzzerChannel);
  ledcAttachPin(rhPin, rhChannel);
}
 
void loop(){
  ledcWrite(rhChannel, 255);
  ledcWrite(buzzerChannel, 255);
}
