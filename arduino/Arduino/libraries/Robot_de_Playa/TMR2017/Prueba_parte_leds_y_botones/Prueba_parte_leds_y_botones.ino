const int LED1 = 32;
const int LED2 = 34;
const int LED3 = 36;
const int LED4 = 38;
const int boton_contador = 48;
const int buzzer = 42;
int contador = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(boton_contador, INPUT);

  Serial.begin(9600);
  
}

void loop(){
  if(digitalRead(boton_contador))
  {
    contador++;
  }
  if(contador > 10)
    digitalWrite(LED1,HIGH);
  if(contador > 30)
    digitalWrite(LED2,HIGH);
  if(contador > 50)
    digitalWrite(LED3,HIGH);
  if(contador > 70)
    digitalWrite(LED4,HIGH);
  if(contador > 100)
  {
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    digitalWrite(LED4,LOW);
    digitalWrite(buzzer,HIGH);
  }
  Serial.println(contador);
  delay(10);
}
