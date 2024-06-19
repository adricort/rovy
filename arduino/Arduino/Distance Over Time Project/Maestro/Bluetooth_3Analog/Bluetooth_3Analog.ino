#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9); // RX, TX

// ASIGNACION DE PINES
const int pot_volumen_pin = A0;
const int pot_ganancia_pin = A1;
const int boton_prueba_pin = 7;

// ASIGNACION DE VARIABLES
int pot_volumen_valor = 0; // [0-1023]
int pot_ganancia_valor = 0;
boolean boton_prueba_valor = false; // [0-1]

// Arreglo para filtro
int valor_anterior;

// CONFIGURACION
void setup() {
  Serial.begin(38400);
  mySerial.begin(38400);
  pinMode(pot_volumen_pin,INPUT);
  pinMode(pot_ganancia_pin,INPUT);
  pinMode(boton_prueba_pin,INPUT);
}

// LOOP PRINCIPAL
void loop() {
  pot_volumen_valor = analogRead(pot_volumen_pin); // [0-1023]
  pot_ganancia_valor = analogRead(pot_ganancia_pin); // [0-1023]
  boton_prueba_valor = digitalRead(boton_prueba_pin); // [0-1]
  
  // REGLA DE 3
  int pot_volumen_valor_mapeado = map(pot_volumen_valor, 0, 1023, 0, 180);
  int pot_ganancia_valor_mapeado = 0;   // Cambiar este a map cuando se solde
  boton_prueba_valor = 0;               // Quitar esta linea cuando se solde el botón

  // Estancamiento de valores para que sea más "suave" la respuesta
  if(pot_volumen_valor_mapeado < 10)
    pot_volumen_valor_mapeado = 0;
  else if(pot_volumen_valor_mapeado > 175)
    pot_volumen_valor_mapeado = 180;

  // FILTRO DE 2 UNIDADES EN UMBRAL
  if(abs(valor_anterior - pot_volumen_valor_mapeado) <= 2)
    pot_volumen_valor_mapeado = valor_anterior;

  // COMUNICACION
  Serial.print("s");Serial.print(pot_volumen_valor_mapeado);
  Serial.print("a");Serial.print(pot_ganancia_valor_mapeado);
  Serial.print("b");Serial.print(boton_prueba_valor);
  Serial.print("c");Serial.println("$");
  
  mySerial.print("s");mySerial.print(pot_volumen_valor_mapeado);
  mySerial.print("a");mySerial.print(pot_ganancia_valor_mapeado);
  mySerial.print("b");mySerial.print(boton_prueba_valor);
  mySerial.print("c");mySerial.println("$");

  valor_anterior = pot_volumen_valor_mapeado;
}
