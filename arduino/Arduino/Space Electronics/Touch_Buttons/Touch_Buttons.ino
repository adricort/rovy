const int led_yellow = 17;  
const int led_green = 16;  
int threshold = 30;

int touch_previousUP = 0;
int touch_previousDOWN = 0;

void setup() {
  #define PIN_TONE 25
  pinMode(PIN_TONE,OUTPUT);
  #define CHANNEL 5
  ledcSetup(CHANNEL, 5000, 8);
  ledcAttachPin(PIN_TONE, CHANNEL);
  ledcWrite(CHANNEL, 0); //duty Cycle de 0
  Serial.begin(115200);
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(33,OUTPUT);
}

void loop() {
  int touchUP = touchRead(T0);
  int touchUP2 = touchRead(T0);
  // digitalizing value for better response:
  if(touchUP < threshold)
    touchUP = 1;
  else
    touchUP= 0;
  if(touchUP2 < threshold)
    touchUP2 = 1;
  else
    touchUP2= 0;

  if(touchUP == 1 && touchUP2 == 1 && touch_previousUP == 0)
  {
    digitalWrite(led_green,HIGH);
    for(int i = 0; i < 5; i++)
    {
      ledcWriteTone(5, 1000);
      delay(50);
      ledcWriteTone(5, 0);
      delay(50);
    }
    touch_previousUP = 1;
  }
  else if(touchUP == 0 && touchUP2 == 0 && touch_previousUP == 1)
  {
    digitalWrite(led_green,LOW);
    touch_previousUP = 0;
  }
  
  int touchDOWN = touchRead(T3);
  int touchDOWN2 = touchRead(T3);
  // digitalizing value for better response:
  if(touchDOWN < threshold)
    touchDOWN = 1;
  else
    touchDOWN= 0;
  if(touchDOWN2 < threshold)
    touchDOWN2 = 1;
  else
    touchDOWN2= 0;

  if(touchDOWN == 1 && touchDOWN2 == 1 && touch_previousDOWN == 0)
  {
    digitalWrite(led_yellow,HIGH);
    for(int i = 0; i < 5; i++)
    {
      ledcWriteTone(5, 500);
      delay(50);
      ledcWriteTone(5, 0);
      delay(50);
    }
    touch_previousDOWN = 1;
  }
  else if(touchDOWN == 0 && touchDOWN2 == 0 && touch_previousDOWN == 1)
  {
    digitalWrite(led_yellow,LOW);
    touch_previousDOWN = 0;
  }
}
