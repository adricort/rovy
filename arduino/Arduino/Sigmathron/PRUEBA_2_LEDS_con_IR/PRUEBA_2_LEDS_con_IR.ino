#include <IRremote.h>

int IRpin = 11;  // pin for the IR sensor
int LED1 = 13;    // LED pin  
int LED2 = 12;
IRrecv irrecv(IRpin);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() 
{
   
  if (irrecv.decode(&results)) 
    {
      irrecv.resume();   // Receive the next value
    }
  
   if (results.value == 2498985700)  // change zero to your IR remote button number
     {
       digitalWrite(LED1, HIGH);         
          
     }
   if (results.value == 602597570)  // change zero to your IR remote button number
     {
       digitalWrite(LED1, LOW);         
          
     }
   if (results.value == 4129126878)  // change zero to your IR remote button number
     {
       digitalWrite(LED2, HIGH);         
          
     }
   if (results.value == 2172071812)  // change zero to your IR remote button number
     {
       digitalWrite(LED2, LOW);         
          
     }

}
