const int led_red = 33;
const int sw3_btn = 27;
volatile bool switchState = 0;

void setup() {
  pinMode(led_red,OUTPUT);
  pinMode(sw3_btn,INPUT);
  attachInterrupt(digitalPinToInterrupt(sw3_btn), ISR, CHANGE);
}

void ISR()
{
  switchState = !switchState;
  digitalWrite(led_red,switchState);
}

void loop() {
  delay(1000);
}
