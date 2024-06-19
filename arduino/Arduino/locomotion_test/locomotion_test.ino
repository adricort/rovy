const int mot_pin[4] = {2,3,4,5};

void setup() {
  for(int i = 0; i < 4; i++)
    pinMode(mot_pin[i], OUTPUT);
}

void loop() {
  analogWrite(mot_pin[0], 0);
  analogWrite(mot_pin[1], 0);
  analogWrite(mot_pin[2], 0);
  analogWrite(mot_pin[3], 0);
}
