const int led_yellow = 17;  
const int led_green = 16;  
int threshold = 30;

void setup() {
  Serial.begin(115200);
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
}

void loop() {
  Serial.print("Up: ");
  Serial.print(touchRead(T0));
  Serial.print(" Down: ");
  Serial.println(touchRead(T3));
  if(touchRead(T0) < threshold)
  {
    digitalWrite(led_yellow,HIGH);
    Serial.println("HEEEEEEEEEEEEEEEEY");
  }
  else
    digitalWrite(led_yellow,LOW);
  if(touchRead(T3) < threshold)
  {
    digitalWrite(led_green,HIGH);
    Serial.println("HEEEEEEEEEEEEEEEEY");
  }
  else
    digitalWrite(led_green,LOW);

  delay(10);
}
