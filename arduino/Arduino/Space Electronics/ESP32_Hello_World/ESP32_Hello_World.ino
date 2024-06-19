const int led_red = 33; // GPIO 33 connected to red LED on PCB
const int led_yellow = 17;  
const int led_green = 16;  

void setup() {
  pinMode(led_red,OUTPUT);
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello World");
}

void loop() {
  Serial.println("Traffic light cycle start");
  digitalWrite(led_red,HIGH);
  digitalWrite(led_yellow,LOW);
  digitalWrite(led_green,LOW);
  delay(1000);
  digitalWrite(led_red,LOW);
  digitalWrite(led_yellow,HIGH);
  digitalWrite(led_green,LOW);
  delay(1000);
   digitalWrite(led_red,LOW);
  digitalWrite(led_yellow,LOW);
  digitalWrite(led_green,HIGH);
  delay(1000);
}
