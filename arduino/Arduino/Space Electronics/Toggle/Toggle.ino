const int green_led_pin = 16;
const int sw3_pin = 27;
int counter = 0;
int sw3_previous_state = 0;
int sw3_state = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sw3_pin,INPUT_PULLUP);
  pinMode(green_led_pin,OUTPUT);
}

void loop() {
  sw3_state = digitalRead(sw3_pin);
  digitalWrite(green_led_pin,!sw3_state);
  if (sw3_previous_state == 0 && sw3_state == 1)
  {
    sw3_previous_state = 1;
    counter++;
    Serial.print("toggle counter: ");
    Serial.println(counter);
  }
  else if (sw3_previous_state ==1 && sw3_state == 0)
  {
    sw3_previous_state = 0;
  }
  delay(10);
}
