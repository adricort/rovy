/* CLUB DE ROBÓTICA DE LA FACULTAD DE INGENIERÍA- Equipo: SPARTAN CROFI
                                                                          PROGRAMA DEL SEGUNDO ARDUINO DZULÚM (ROVY). CATEGORÍA: ROBO MAGELLAN
                                                                                    SECCIÓN DE POTENCIA
Valores de PWM para los motores:
Adelante: 197-253   Donde el 197 es el más lento y el 253 es el más rápido                              Desarrollado por: Ricárdez Ortigosa Adrián
Atrás:    1-185     Donde el 185 es el más lento y el 1 es el más rápido
Paro:     186-196   El promedio de PARO es 191

Materiales necesarios:
- 1 brújula GY-271
- 2 Arduinos UNO

Conexiones además de VCC y GND:
Segundo Arduino        Brújula
A4          -->          SDA
A5          -->          SCL
Segundo Arduino    Primer Arduino
pin 7          -->   pin 6
pin 6          -->   pin 7
Primer Arduino    Segundo Arduino
pin 8          -->  Reset
*/

// BIBLIOTECAS
#include <Wire.h>
#include <SoftwareSerial.h>       // Biblioteca necesaria para multiplexear los puertos seriales del Arduino
#include <Adafruit_Sensor.h>      // Biblioteca para la brújula
#include <Adafruit_HMC5883_U.h>   // Biblioteca para la brújula

// MULTIPLEXEO DE PUERTO SERIAL
SoftwareSerial Arduinos(6,7);     // Arduinos(Rx,Tx)

// VARIABLES GLOBALES
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345); // Asigna una ID única para este sensor al mismo tiempo
float headingz = 0;
float headingy = 45;
float headingx = 90;
float Gradosz;                   // Variable del ángulo actual del robot (la brújula)
float thetha_grados;             // Ángulo que va a recibir vía Serial del PRIMER ARDUINO
float PWM_XCTU_Izq=191, PWM_XCTU_Der=191;   // Variables para los motores de Rovy, empiezan en "PARO"
const int Mot_Izq = 10, Mot_Der = 11;     // Pines digitales del Arduino para los PWM de los Motores

// ============================================
// ============== VOID SETUP ==================
// ============================================
void setup(){
  // VELOCIDAD DE PUERTOS SERIALES
  Serial.begin(9600);   // Serial Monitor
  Arduinos.begin(9600); // Serial Arduinos
  if(!mag.begin())    // Inicializa el objeto mag de la brújula
  {
    while(1);
  }
  displaySensorDetails();
}

// ===========================================
// ============== VOID LOOP ==================
// ===========================================
void loop() 
{
  Serial.println("Esperando a recibir el angulo de direccion del PRIMER ARDUINO...");

  while(Arduinos.available()==0){         // Espera la respuesta del dato que el primer Arduino va a dar por el puerto serial Arduinos(Rx,Tx)      
  }
  thetha_grados= Arduinos.parseFloat();   // Lee únicamente los números flotantes que recibe del primer Arduino del Arduinos.print(theta_grados)
  // Conversión de ángulos de "0° a 360°" a "0 a 180° y -180° a 0°"
  if( thetha_grados > 180 )
    thetha_grados= thetha_grados-360;
  
  Serial.print("El angulo de direccion en grados es de:  ");
  Serial.print(thetha_grados);Serial.println("  grados");  // Del Ejemplo: 64.5379°
  //delay(3000);

  Serial.println("");
  Serial.println("A continuacion, viene la calibracion de la brujula, espere un momento porfavor...");
  Serial.println("");
  //delay(3000);
  
  for(int i=0;i<500;i++){    // Empezará de 0, y terminará en 500         
    brujulazo();
    float alfa_grados= Gradosz*(-1)+180;  // 
    
    // Conversión de ángulos de "0° a 360°" a "0 a 180° y -180° a 0°"
    if( alfa_grados > 180 )
      alfa_grados = alfa_grados-360;
     
    Serial.println(alfa_grados);
  }

  Serial.println("");
  Serial.println("Ha terminado la calibracion, gracias por la espera.");
  //delay(3000);
  Serial.println("Ahora comenzara el programa principal de correccion de MOTORES");
  //delay(3000);
  
  Serial.print("Angulo del robot,");
  Serial.print("PWM_XCTU_Izq,");
  Serial.print("PWM_XCTU_Der");
  //delay(3000);

  while(true){
    brujulazo();
    float alfa_grados= Gradosz*(-1)+180;  // CHECAR ESTO
    
    // Conversión de ángulos de "0° a 360°" a "0 a 180° y -180° a 0°"
    if( alfa_grados > 180 )
      alfa_grados = alfa_grados-360;
    
    PWM_XCTU_Izq= 225+(alfa_grados-thetha_grados)*0.717;    // 0.717 fue el factor más grande para que avance 253 PWM a 60° de diferencia entre ángulos
    PWM_XCTU_Der= 225+(thetha_grados-alfa_grados)*0.717;    // 0.717 fue el factor más grande para que avance 253 PWM a 60° de diferencia entre ángulos
    // Límites de avance, porque si no, comienza a hacer estupideces el robot XD
    if( PWM_XCTU_Izq >= 253 )
      PWM_XCTU_Izq = 253;
    if( PWM_XCTU_Der >= 253 )
      PWM_XCTU_Der = 253;
      
    // POTENCIA
    analogWrite(Mot_Izq,PWM_XCTU_Izq);
    analogWrite(Mot_Der,PWM_XCTU_Der);
    Serial.print(alfa_grados);Serial.print(",");
    Serial.print(PWM_XCTU_Izq);Serial.print(",");
    Serial.println(PWM_XCTU_Der);
  }
}

// FUNCIONES ------------------------------------------------------------------------------------------
void displaySensorDetails()
{
  sensor_t sensor;
  mag.getSensor(&sensor);
}

void brujulazo(){
  // COSAS PARA QUE FUNCIONE LA BRÚJULA XD
  sensors_event_t event; 
  mag.getEvent(&event);

  headingz = atan2(event.magnetic.y, event.magnetic.x);
  
  float declinationAngle = 0.22;
  
  headingz += declinationAngle;
  
  //Para girar en el eje Z solo de 0-360°
  if(headingz < 0)
    headingz += 2*PI;
    
  if(headingz > 2*PI)
    headingz -= 2*PI;
    
  Gradosz = headingz * 180/M_PI-8;    // Corrección de 8 grados porque puse mal la brújula XD
  
  // FIN COSAS PARA QUE FUNCIONE LA BRÚJULA XD
}

