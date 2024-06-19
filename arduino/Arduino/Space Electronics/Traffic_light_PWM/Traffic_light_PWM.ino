const int green_led_pin = 16;
const int yellow_led_pin = 17;

void setup() {
  pinMode(green_led_pin,OUTPUT);
  pinMode(yellow_led_pin,OUTPUT);
  ledcSetup(0,5000,8);
  ledcAttachPin(green_led_pin,0);
  ledcAttachPin(yellow_led_pin,1);
  ledcWrite(0, 255);
  ledcWrite(1, 0);
}

void loop() {
  for(int i = 255; i > 0; i--)
  {
    ledcWrite(0, i);
    delay(5);
  }
  for(int i = 0; i < 255; i++)
  {
    ledcWrite(1, i);
    delay(5);
  }
  for(int i = 255; i > 0; i--)
  {
    ledcWrite(1, i);
    delay(5);
  }
  for(int i = 0; i < 255; i++)
  {
    ledcWrite(0, i);
    delay(5);
  }
}
