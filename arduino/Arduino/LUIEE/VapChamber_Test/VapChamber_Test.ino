int PWM = 155;

void setup() {
  Serial.begin(57600);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  Serial.print("Write the desired PWM: ");
}

void loop() {
  /*analogWrite(8,PWM);     // left bottom forwards
  analogWrite(7,0);       // left bottom backwards
  analogWrite(9,PWM);    // right forwards
  analogWrite(10,0);      // right backwards
  analogWrite(11,PWM);    // left top forwards
  analogWrite(12,0);      // left top backwards
  delay(1000);
  analogWrite(8,0);       // alone forwards
  analogWrite(7,0);     // alone backwards
  analogWrite(10,0);      // couple1 forwards
  analogWrite(9,0);    // couple1 backwards
  analogWrite(11,0);      // couple2 forwards
  analogWrite(12,0);    // couple2 backwards 
  delay(2000);*/
  analogWrite(8,0);          // 1 forwards
  analogWrite(7,PWM);       // 1 backwards
  analogWrite(9,0);             // 2 forwards
  analogWrite(10,PWM);     // 2 backwards
  analogWrite(11,0);      // 3 forwards
  analogWrite(12,PWM);    // 3 backwards
  delay(2000);
  while(1)
  {
    analogWrite(8,0);       // alone forwards
    analogWrite(7,0);     // alone backwards
    analogWrite(10,0);      // couple1 forwards
    analogWrite(9,0);    // couple1 backwards
    analogWrite(11,0);      // couple2 forwards
    analogWrite(12,0);    // couple2 backwards
  }
}
