void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  if (Serial1.available() > 0)
  {
    String str = Serial1.readStringUntil(36);
    Serial.println(str);
  }
}
