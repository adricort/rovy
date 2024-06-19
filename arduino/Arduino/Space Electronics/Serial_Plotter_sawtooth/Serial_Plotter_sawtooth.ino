void setup() {
  Serial.begin(9600);
}

void loop() {
  // Triggering a sawtooth with a character
  if(Serial.available())
  {
    if(Serial.read() == 'a')
      for(int i = 0; i < 255; i++)
      {
        Serial.println(i);
        delay(10);    //2.5s/255 = ~10ms
      }
  }
}
