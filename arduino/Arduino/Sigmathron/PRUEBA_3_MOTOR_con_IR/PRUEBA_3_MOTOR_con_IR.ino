#include <IRremote.h>

int IRpin = 11;  // pin for the IR sensor
int MOTOR = 13;    // LED pin  
IRrecv irrecv(IRpin);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(MOTOR, OUTPUT);
}

void loop() 
{
   
  if (irrecv.decode(&results)) 
    {
      irrecv.resume();   // Receive the next value
    }
  
   if (results.value == 2498985700)  // change zero to your IR remote button number
     {
       digitalWrite(MOTOR, HIGH);         
          
     }
   if (results.value == 602597570)  // change zero to your IR remote button number
     {
       digitalWrite(MOTOR, LOW);         
          
     }

}
