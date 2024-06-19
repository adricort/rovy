#include <IRremote.h>

// PINES Y RECEPTOR IR

int IRpin = 12;
IRrecv irrecv(IRpin);
decode_results results;
const byte IN2pin = 2;
const byte IN3pin = 3;
const byte IN5pin = 5;
const byte IN6pin = 6;
const byte IN7pin = 7;
const byte IN8pin = 8;
const byte IN9pin = 9;

void setup()
{
  // Receptor IR
  
  Serial.begin(9600);
  irrecv.enableIRIn();   // Comienza a recibir
  pinMode (PIN1, OUTPUT);
  pinMode (PIN2, OUTPUT);
}

void loop() 
{
  // DECODIFICADOR IR
   
  if (irrecv.decode(&results)) 
    {
      Serial.println(results.value, DEC);   //Decodifica
      irrecv.resume();   // Recibe el siguiente valor
    }
    
  // MOTORES
  
  if (results.value == 2287677653 or results.value == 2717196066)   // NUMERO 1 (motor izquierdo hacia adelante)
     {
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, HIGH);           
     }
     
  if (results.value == 1727565 or results.value == 1044583754)   // NUMERO 7 (motor izquierdo hacia atras)
     {
       digitalWrite(IN3pin, HIGH);  
       digitalWrite(IN5pin, LOW);       
          
     }
     
  if (results.value == 2739886593 or results.value == 2141887130)   // NUMERO 3 (motor derecho hacia adelante)
     {
       digitalWrite(IN6pin, LOW); 
       digitalWrite(IN2pin, HIGH);         
     }
     
  if (results.value == 2504251343 or results.value == 4239022680)   // NUMERO 9 (motor derecho hacia atras)
     {
       digitalWrite(IN6pin, HIGH);     
       digitalWrite(IN2pin, LOW);
     }
     
  if (results.value == 2529536171 or results.value == 3674269916)   // NUMERO 5 (los 2 motores se apagan)
     {
       digitalWrite(IN2pin, LOW);
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, LOW);     
       digitalWrite(IN6pin, LOW);      
     }
     
  if (results.value == 3890270129 or results.value == 4087990600)   // NUMERO 2 (los 2 motores hacia adelante)
     {
       digitalWrite(IN2pin, HIGH);  
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, HIGH);  
       digitalWrite(IN6pin, LOW);             
     }
     
  if (results.value == 1470527121 or results.value == 3088639144)   // NUMERO 8 (los 2 motores hacia atras)
     {
       digitalWrite(IN2pin, LOW);  
       digitalWrite(IN3pin, HIGH);
       digitalWrite(IN5pin, LOW);  
       digitalWrite(IN6pin, HIGH);       
     }
     
  if (results.value == 576802165 or results.value == 2523886380)   // NUMERO 4 (rotacion hacia la izquierda)
     {
       digitalWrite(IN2pin, LOW);  
       digitalWrite(IN3pin, HIGH);
       digitalWrite(IN5pin, LOW);  
       digitalWrite(IN6pin, HIGH);       
     }
     
  if (results.value == 1152111101 or results.value == 2071677440)   // NUMERO 6 (rotacion hacia la derecha)
     {
       digitalWrite(IN2pin, LOW);  
       digitalWrite(IN3pin, HIGH);
       digitalWrite(IN5pin, LOW);  
       digitalWrite(IN6pin, HIGH);       
     }
     
  // CIERRA ELECTRICA
     
  if (results.value == 1259510059 or results.value == 2115418426)   // VOL+
     {
       digitalWrite(IN7pin, HIGH);
     }
     
  if (results.value == 468240397 or results.value == 3265801962)   // VOL-
     {
       digitalWrite(IN7pin, LOW);
     }
     
  // LANZALLAMAS
     
  if (results.value == 2970385547 or results.value == 209284604)   // CH>
     {
       digitalWrite(IN8pin, HIGH);
       digitalWrite(IN9pin, HIGH);
       
     }
     
  if (results.value == 278010727 or results.value == 3679522646)   // CH<
     {
       digitalWrite(IN8pin, LOW);
       digitalWrite(IN9pin, LOW);
     }
     
  // DETENCION TOTAL
  
  if (results.value == 2834997245 or results.value == 3841412698)   // ENTER
     {
       digitalWrite(IN2pin, LOW);
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, LOW);     
       digitalWrite(IN6pin, LOW);
       digitalWrite(IN7pin, LOW);
       digitalWrite(IN8pin, LOW);
       digitalWrite(IN9pin, LOW);
     }
  
  // DEMO
}


