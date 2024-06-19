const int led_pin_array[] = {16,17,33}; // {green pin,yellow pin,red pin}

const int symbol_delay_array[] = {100,200,300};

void setup() {
  pinMode(led_pin_array[0],OUTPUT);
  pinMode(led_pin_array[1],OUTPUT);
  pinMode(led_pin_array[2],OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello World");
}

// Dots function
void dot(int n_dots,int t, int pin)
{
  for(int i = 0; i < n_dots; i++)
  {
    digitalWrite(pin,HIGH);
    delay(t);
    digitalWrite(pin,LOW);
    delay(t);
  }
}

// Dashes function
void dash(int n_dashes,int t, int pin)
{
  for(int i = 0; i < n_dashes; i++)
  {
    digitalWrite(pin,HIGH);
    delay(t*3);
    digitalWrite(pin,LOW);
    delay(t*3);
  }
}
// D function
void D(int t, int pin)
{
  dash(1,t,pin);
  dot(4,t,pin);
  delay(t*3);
}

// E function
void E(int t, int pin)
{
  dot(1,t,pin);
  delay(t*3);
}

// H function
void H(int t, int pin)
{
  dot(4,t,pin);
  delay(t*3);
}

// L function
void L(int t, int pin)
{
  dot(1,t,pin);
  dash(1,t,pin);
  dot(2,t,pin);
  delay(t*3);
}

// O function
void O(int t, int pin)
{
  dash(3,t,pin);
  delay(t*3);
}

// R function
void R(int t, int pin)
{
  dot(1,t,pin);
  dash(1,t,pin);
  dot(1,t,pin);
  delay(t*3);
}

// W function
void W(int t, int pin)
{
  dot(1,t,pin);
  dash(2,t,pin);
  delay(t*3);
}

// I function
void I(int t, int pin)
{
  dot(2,t,pin);
  delay(t*3);
}

void loop() 
{
  /*
  // Task 1 and 2 : HI
  Serial.println("Green station operating");
  H(symbol_delay_array[0],led_pin_array[0]);
  delay(symbol_delay_array[0]*7);
  I(symbol_delay_array[0],led_pin_array[0]);
  delay(symbol_delay_array[0]*7);

  Serial.println("Red station operating");
  H(symbol_delay_array[2],led_pin_array[2]);
  delay(symbol_delay_array[2]*7);
  I(symbol_delay_array[2],led_pin_array[2]);
  delay(symbol_delay_array[2]*7);*/

  // Task 3 : HELLO WORLD
  Serial.println("Red station operating");
  H(symbol_delay_array[2],led_pin_array[2]);
  E(symbol_delay_array[2],led_pin_array[2]);
  L(symbol_delay_array[2],led_pin_array[2]);
  L(symbol_delay_array[2],led_pin_array[2]);
  O(symbol_delay_array[2],led_pin_array[2]);
  delay(symbol_delay_array[0]*7);
  W(symbol_delay_array[2],led_pin_array[2]);
  O(symbol_delay_array[2],led_pin_array[2]);
  R(symbol_delay_array[2],led_pin_array[2]);
  L(symbol_delay_array[2],led_pin_array[2]);
  D(symbol_delay_array[2],led_pin_array[2]);
  delay(symbol_delay_array[0]*7);
}
