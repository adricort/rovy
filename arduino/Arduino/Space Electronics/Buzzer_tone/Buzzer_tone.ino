#define SERIAL_BAUDRATE 115200
#define PIN_TONE 25

void setup() {
  pinMode(25,OUTPUT);
  #define CHANNEL 5
  ledcSetup(CHANNEL, 5000, 8);
  ledcAttachPin(PIN_TONE, CHANNEL);
  ledcWrite(CHANNEL, 0); //duty Cycle de 0

}

void loop() {
  tone(25,1000);
  delay(1000);
  noTone(25);
  delay(1000);
}

void tone(int pin, int frequency) //FOR ESP Platform, pin is unused
{
    ledcWriteTone(CHANNEL, frequency);
}
void noTone(int pin) //FOR ESP Platform, pin is unused
{
    ledcWrite(CHANNEL, 0);
}
