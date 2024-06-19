#include <IRremote.h>

// PINES Y RECEPTOR IR

int IRpin = 12;
int i;
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
  
  if (results.value == 16)   // NUMERO 1 (motor izquierdo hacia adelante)
     {
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, HIGH);           
     }
     
  if (results.value == 1552)   // NUMERO 7 (motor izquierdo hacia atras)
     {
       digitalWrite(IN3pin, HIGH);  
       digitalWrite(IN5pin, LOW);       
          
     }
     
  if (results.value == 1040)   // NUMERO 3 (motor derecho hacia adelante)
     {
       digitalWrite(IN6pin, LOW); 
       digitalWrite(IN2pin, HIGH);         
     }
     
  if (results.value == 272)   // NUMERO 9 (motor derecho hacia atras)
     {
       digitalWrite(IN6pin, HIGH);     
       digitalWrite(IN2pin, LOW);
     }
     
  if (results.value == 528)   // NUMERO 5 (los 2 motores se apagan)
     {
       digitalWrite(IN2pin, LOW);
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, LOW);     
       digitalWrite(IN6pin, LOW);      
     }
     
  if (results.value == 2064)   // NUMERO 2 (los 2 motores hacia adelante)
     {
       digitalWrite(IN2pin, HIGH);  
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, HIGH);  
       digitalWrite(IN6pin, LOW);             
     }
     
  if (results.value == 3600)   // NUMERO 8 (los 2 motores hacia atras)
     {
       digitalWrite(IN2pin, LOW);  
       digitalWrite(IN3pin, HIGH);
       digitalWrite(IN5pin, LOW);  
       digitalWrite(IN6pin, HIGH);       
     }
     
  if (results.value == 3088)   // NUMERO 4 (giro hacia la izquierda)
     {
       digitalWrite(IN2pin, HIGH);  
       digitalWrite(IN3pin, HIGH);
       digitalWrite(IN5pin, LOW);  
       digitalWrite(IN6pin, LOW);      
     }
     
  if (results.value == 2576)   // NUMERO 6 (giro hacia la derecha)
     {
       digitalWrite(IN2pin, LOW);  
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, HIGH);  
       digitalWrite(IN6pin, HIGH); 
     }
     
  // SIERRA ELECTRICA
     
  if (results.value == 1168)   // VOL+ (sierra electrica prendida)
     {
       digitalWrite(IN7pin, HIGH);
     }
     
  if (results.value == 3216)   // VOL- (sierra electrica apagada)
     {
       digitalWrite(IN7pin, LOW);
     }
     
  // LANZALLAMAS
     
  if (results.value == 144)   // CH> (lanzallamas prendido)
     {
       digitalWrite(IN8pin, HIGH);
       digitalWrite(IN9pin, HIGH);
       
     }
     
  if (results.value == 2192)   // CH< (lanzallamas apagado)
     {
       digitalWrite(IN8pin, LOW);
       digitalWrite(IN9pin, LOW);
     }
     
  // DETENCION TOTAL
  
  if (results.value == 2704)   // ENTER (todos los pines se apagan)
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
  
  if (results.value == 2672)   // ???
     {
       digitalWrite(IN2pin, HIGH);   // GIRO IZQUIERDO
       digitalWrite(IN3pin, HIGH);
       digitalWrite(IN5pin, LOW);  
       digitalWrite(IN6pin, LOW);
       delay(3500);
       
       digitalWrite(IN2pin, LOW);   // GIRO DERECHO
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN5pin, HIGH);  
       digitalWrite(IN6pin, HIGH);
       delay(3400);
       
       digitalWrite(IN6pin, LOW);   // MOTOR IZQUIERDO HACIA ADELANTE
       delay(1100);
       
       digitalWrite(IN2pin, HIGH);   // LOS 2 MOTORES HACIA ADELANTE
       digitalWrite(IN7pin, HIGH);   // SIERRA ELECTRICA PRENDIDA
       delay(3000);
       
       digitalWrite(IN5pin, LOW);   // MOTOR DERECHO HACIA ADELANTE
       digitalWrite(IN7pin, LOW);   // SIERRA ELECTRICA APAGADA
       delay(1350);
       
       digitalWrite(IN5pin, HIGH);   // LOS 2 MOTORES HACIA ADELANTE
       digitalWrite(IN7pin, HIGH);   // SIERRA ELECTRICA PRENDIDA
       delay(3000);
       
       digitalWrite(IN5pin, LOW);   // MOTOR DERECHO HACIA ADELANTE
       digitalWrite(IN7pin, LOW);   // SIERRA ELECTRICA APAGADA
       delay(1350);
       
       digitalWrite(IN5pin, HIGH);   // LOS 2 MOTORES HACIA ADELANTE
       digitalWrite(IN7pin, HIGH);   // SIERRA ELECTRICA PRENDIDA
       delay(3000);
       
       digitalWrite(IN5pin, LOW);   // MOTOR DERECHO HACIA ADELANTE
       digitalWrite(IN7pin, LOW);   // SIERRA ELECTRICA APAGADA
       delay(1350);
       
       digitalWrite(IN5pin, HIGH);   // LOS 2 MOTORES HACIA ADELANTE
       digitalWrite(IN7pin, HIGH);   // SIERRA ELECTRICA PRENDIDA
       delay(3000);
       
       digitalWrite(IN5pin, LOW);   // MOTOR DERECHO HACIA ADELANTE (va hacia el centro)
       digitalWrite(IN7pin, LOW);   // SIERRA ELECTRICA APAGADA
       delay(2000);
       
       digitalWrite(IN5pin, HIGH);   // LOS 2 MOTORES HACIA ADELANTE (llega al centro)
       delay(2500);
       
       digitalWrite(IN2pin, HIGH);   // GIRO IZQUIERDO (final)
       digitalWrite(IN3pin, HIGH);
       digitalWrite(IN5pin, LOW);  
       digitalWrite(IN6pin, LOW);
       digitalWrite(IN7pin, HIGH);   // SIERRA ELECTRICA PRENDIDA
       delay(4400);
       
       digitalWrite(IN2pin, LOW);   // TERMINA GIRO Y SIERRA ELECTRICA APAGADA
       digitalWrite(IN3pin, LOW);
       digitalWrite(IN7pin, LOW);
       delay(2000);
       
       digitalWrite(IN8pin, HIGH);   // LANZALLAMAS PRENDIDO
       digitalWrite(IN9pin, HIGH);
       delay(4000);
       
       for(i=0;i<2;i++)
       {
         digitalWrite(IN2pin, LOW);   // DEMO OFF
         digitalWrite(IN3pin, LOW);
         digitalWrite(IN5pin, LOW);     
         digitalWrite(IN6pin, LOW);
         digitalWrite(IN7pin, LOW);
         digitalWrite(IN8pin, LOW);
         digitalWrite(IN9pin, LOW);
         i=0;
       }
     }
}


