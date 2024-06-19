// TERMÓMETRO DIGITAL (LM35- SENSOR DE TEMPERATURA, DISPLAY DOBLE DE 7 SEGMENTOS)

const int A= 6;    // PINES PARA NUMEROS BINARIOS PARA EL INTEGRADO SN74LS47
const int B= 7;
const int C= 8;
const int D= 9;

const int E= 2;
const int F= 3;
const int G= 4;
const int H= 5;

int temperatura;
int miliVolts;
const int LM35= A0;

// FUNCIÓN PRIMER DISPLAY

void PrimerDisplay() {
  switch(temperatura){
    case 0:
    case 10:
    case 20:
    case 30:
      //NÚMERO 0
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
    case -1:
    case 1:
    case 11:
    case 21:
    case 31:
      //NÚMERO 1
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
    case -2:
    case 2:
    case 12:
    case 22:
      //NÚMERO 2
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
    case -3:
    case 3:
    case 13:
    case 23:
    case 33:
      //NÚMERO 3
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
     case -4:
     case 4:
     case 14:
     case 24:
     case 34:
      //NÚMERO 4
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
     case -5:
     case 5:
     case 15:
     case 25:
     case 35:
      //NÚMERO 5
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
    case -6:
    case 6: 
    case 16:
    case 26:
      //NÚMERO 6
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
    case -7:
    case 7:    
    case 17:
    case 27:
      //NÚMERO 7
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
    case -8:
    case 8:  
    case 18:
    case 28:
      //NÚMERO 8
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      break;
    case -9:
    case 9:  
    case 19:
    case 29:
      //NÚMERO 9
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      break;
  }
}

// FUNCIÓN SEGUNDO DISPLAY

void SegundoDisplay() {
  if(temperatura < 0){
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, HIGH);
    digitalWrite(H, LOW);
  }
  if(temperatura >=0 && temperatura <10){
    digitalWrite(E, LOW);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    digitalWrite(H, LOW);
  }
  if(temperatura >=10 && temperatura < 20){
    digitalWrite(E, HIGH);
    digitalWrite(F, LOW);
    digitalWrite(G, LOW);
    digitalWrite(H, LOW);
  }
  if(temperatura >=20 && temperatura < 30){
    digitalWrite(E, LOW);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(H, LOW);
  }
  if(temperatura >=30 && temperatura < 40){
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(H, LOW);
  }
}

void setup(){
  Serial.begin(9600);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);
  pinMode(LM35, INPUT);
  
  Serial.println(temperatura);
  miliVolts= (analogRead(LM35)*5000L)/1023; // Regla de 3, ya que por cada 1°C da 10mV el sensor LM35
  temperatura= miliVolts/10-1.5; // Temperatura= mV/10, le puse -2 porque tiene un error aproximado de 5 unidades en la temperatura
  PrimerDisplay();
  SegundoDisplay();
}

void loop(){
  Serial.println(temperatura);
  miliVolts= (analogRead(LM35)*5000L)/1023;
  temperatura= miliVolts/10-1.5;
  delay(5000);
  PrimerDisplay();
  SegundoDisplay();
}


