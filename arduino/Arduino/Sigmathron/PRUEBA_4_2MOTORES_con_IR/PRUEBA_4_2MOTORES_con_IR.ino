#include <IRremote.h>

int IRpin = 13;
IRrecv irrecv(IRpin);
decode_results results;
const byte IN1pin = 2;
const byte IN2pin = 3;
const byte IN3pin = 4;
const byte IN4pin = 5;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode (PIN1, OUTPUT);
  pinMode (PIN2, OUTPUT);
}

void loop() 
{
   
  if (irrecv.decode(&results)) 
    {
      Serial.println(results.value, DEC);
      irrecv.resume();   // Receive the next value
    }
  
   if (results.value == 2498985700)  // NUMERO 3 (motor izquierdo hacia adelante)
     {
       digitalWrite(IN1pin, LOW);
       digitalWrite(IN2pin, HIGH);
                 
     }
   if (results.value == 2513627108)  // NUMERO 9 (motor izquierdo hacia atras)
     {
       digitalWrite(IN1pin, HIGH);  
       digitalWrite(IN2pin, LOW);       
          
     }
   if (results.value == 2172071812)  // NUMERO 1 (motor derecho hacia adelante)
     {
       digitalWrite(IN3pin, LOW); 
       digitalWrite(IN4pin, HIGH);      
          
     }
   if (results.value == 3430842824)  // NUMERO 7 (motor derecho hacia atras)
     {
       digitalWrite(IN3pin, HIGH);     
       digitalWrite(IN4pin, LOW );    
          
     }
   if (results.value == 3345571616)  // NUMERO 5 (los 2 motores se apagan)
     {
       digitalWrite(IN1pin, LOW);
       digitalWrite(IN2pin, LOW);
       digitalWrite(IN3pin, LOW);     
       digitalWrite(IN4pin, LOW);    
          
     }
   if (results.value == 4129126878)  // NUMERO 2 (los 2 motores hacia adelante)
     {
       digitalWrite(IN1pin, LOW);  
       digitalWrite(IN2pin, HIGH);
       digitalWrite(IN3pin, LOW);  
       digitalWrite(IN4pin, HIGH);       
          
     }
   if (results.value == 324244318)  // NUMERO 8 (los 2 motores hacia atras)
     {
       digitalWrite(IN1pin, HIGH);  
       digitalWrite(IN2pin, LOW);
       digitalWrite(IN3pin, HIGH);  
       digitalWrite(IN4pin, LOW);       
          
     }


}


