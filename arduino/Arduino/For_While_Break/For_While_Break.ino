/* PROGRAMA PARA COMPROBAR BREAK, WHILE, Y FOR

Conectar al Arduino UNO:
1 push button con resistor de 1-10K Ohms, y entrada al PIN 7 en PULL-DOWN
1 push button con resistor de 1-10K Ohms, y entrada al PIN 8 en PULL-DOWN
+5V y GND
*/

const int LED = 13;
const int boton_1 = 7;
const int boton_2 = 8;
boolean cont = false;
int i;

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(boton_1,INPUT);
  pinMode(boton_2,INPUT);
  Serial.begin(9600);
}

void loop() {
  
  Serial.println("NO HA ENTRADO AL WHILE");
  
  if(digitalRead(boton_1) == HIGH){
    Serial.println("YA ENTRO AL WHILE!");
    for(i=0;i<10;i++){                  // FUNCIONA IGUAL QUE OTRO WHILE (TRUE), un break sólo rompe 1 de estos ciclos
      Serial.println(i);
      if(i == 8){
        break;      // Este break, sólo romperá el for de arriba
      }
      while(true){
        digitalWrite(LED,HIGH);
        Serial.println("LED PRENDIDO");
        delay(500);
        digitalWrite(LED,LOW);
        Serial.println("LED APAGADO");
        delay(500);
        if(digitalRead(boton_2) == HIGH){
          cont = true;
          break;      // Este break, sólo romperá el while(true) de arriba
        } // FIN IF
      } // FIN WHILE 
    } // FIN FOR
  } // FIN IF
} // FIN LOOP

