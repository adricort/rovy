void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
}

void loop() {
  analogWrite(2,0);
  analogWrite(3,255);
  analogWrite(4,0);
  analogWrite(5,255);
}
