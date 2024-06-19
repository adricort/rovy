String frame1 = "";
String frame2 = "";

void setup(void)
{
  Serial1.begin(115200);
  Serial.begin(115200);
}

void loop(void)
{
  if (Serial1.available())
  {
    frame1 = Serial1.readStringUntil(37);
    Serial.println(frame1);
  }
}
