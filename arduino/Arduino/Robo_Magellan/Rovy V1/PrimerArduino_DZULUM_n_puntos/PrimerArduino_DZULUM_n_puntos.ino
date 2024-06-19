/* CLUB DE ROBÓTICA DE LA FACULTAD DE INGENIERÍA- Equipo: SPARTAN CROFI
                                                                          PROGRAMA DEL PRIMER ARDUINO DZULÚM (ROVY). CATEGORÍA: ROBO MAGELLAN
                                                                                            SECCIÓN DE DATOS
Coordenadas formato Google Earth para ejemplificar: 
Inicial: 19 18.3253N,99 09.2830W  (casa afuera)                                               Desarrollado por: Ricárdez Ortigosa Adrián
Final: 19 18.3358N,99 09.2795W    (en la coladera de enmedio)
Final Calle: 19 18.3545N,99 09.2705W

Materiales necesarios:
- 1 Ultimate Adafruit GPS Breakout V3
- 2 Arduinos UNO

Conexiones además de VCC y GND:
Primer Arduino        GPS
pin 4          -->    Tx
pin 5          -->    Rx
Primer Arduino    Segundo Arduino
pin 6          -->   pin 7
pin 7          -->   pin 6
Primer Arduino    Primer Arduino
pin 8          -->   pin 2 ("0" Interrupción)


*/

// ==============  BIBLIOTECAS =================
#include <Adafruit_GPS.h>               // Biblioteca necesaria para el Ultimate Adafruit GPS Breakout V3
#include <SoftwareSerial.h>             // Biblioteca necesaria para multiplexear los puertos seriales del Arduino

// ======== MULTIPLEXEO DE PUERTOS SERIALES ==========
SoftwareSerial GPSerial(4,5);     // (Rx,Tx)
SoftwareSerial Arduinos(6,7);     // (Rx,Tx)

Adafruit_GPS GPS(&GPSerial);      // Crea el objeto GPS en base al puerto serial GPSerial

// ========== VARIABLES GLOBALES =================
String NMEA1, NMEA2;              // Variables para las sentencias NMEA del GPS
char c;                           // Variable para leer caracteres del GPS
const int interruptor = 8;        // Pin que generará la interrupción en el Segundo Arduino
const int LED_calibracion_lista = 10; // Pin que indicará que la calibración del GPS ya se hizo
const int camara_check = 11;    // Pin que recibirá el pulso de terminación de la cámara
float degWhole;                   // Variable para toda la parte de posición
float degDec;                     // Variable para la parte decimal de ángulo
float Coord_inicial_lat;          // Coordenada inicial del robot en latitud
float Coord_inicial_long;         // Coordenada inicial del robot en longitud
float deg;                        // Soportará el funcionamiento del Ultimate Adafruit GPS Breakout V3
boolean calibrado = false;        // Variable booleana que determina si ya se calibró una vez el GPS
int i = 0;                        // Para for's

// ============================================
// ============== VOID SETUP ==================
// ============================================
void setup() {
  
  // VELOCIDAD DE PUERTOS SERIALES
  Serial.begin(9600);   // Serial Monitor
  Arduinos.begin(9600); // Serial Arduinos
  GPS.begin(9600);      // Serial GPS
  pinMode(interruptor,OUTPUT);
  pinMode(LED_calibracion_lista,OUTPUT);
  pinMode(camara_check,INPUT);
  
  GPS.sendCommand("$PGCMD,33,0*6D");            //Turn off antenna update nuisance data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Request RMC and GGA Sentences only
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);    //Set update rate to 1 hz
  delay(10);
}

// ===========================================
// ============== VOID LOOP ==================
// ===========================================
void loop(){
  
  // ============= RECEPCIÓN DE DATOS =============
  
  Serial.println("PROGRAMA PRIMER ARDUINO DZULUM (ROVY)- CATEGORIA ROBO MAGELLAN          CLUB DE ROBOTICA DE LA FACULTAD DE INGENIERIA (CROFI)");Serial.println("");
  
  Serial.print("Ingrese la cantidad de puntos objetivo del robot, ejemplo '5':  ");
  while(Serial.available()==0){}                  // Espera la entrada de la cantidad de puntos objetivo del usuario "n"
  int n = Serial.parseInt();      // La variable tomará el valor sólo de la parte de número entero que se ingrese
  Serial.println(n);
  float Coord_punto_lat[n];       // Crea el arreglo de coordenadas de los puntos objetivo en latitud
  float Coord_punto_long[n];      // Crea el arreglo de coordenadas de los puntos objetivo en longitud

  for(i=0;i<n;i++){
    Serial.print("Ingrese la latitud de la coordenada numero ");Serial.print(i+1);Serial.print(" ejemplo '1918.3358':  ");
    while(Serial.available()==0){}                // Espera la entrada de la cantidad de puntos objetivo del usuario
    Coord_punto_lat[i]= Serial.parseFloat();      // La variable tomará el valor sólo de la parte de número flotante que se ingrese
    Serial.println(Coord_punto_lat[i],4);
    
    Serial.print("Ingrese la longitud de la coordenada numero ");Serial.print(i+1);Serial.print(" ejemplo '9909.2795':  ");
    while(Serial.available()==0){}
    Coord_punto_long[i]= Serial.parseFloat();    // La variable tomará el valor sólo de la parte de número flotante que se ingrese
    Serial.println(Coord_punto_long[i],4);
  }
 
  Serial.println("El arreglo de coordenadas objetivo en formato Google Earth queda como:");
  for(i=0;i<n;i++){
    Serial.print("19 ");Serial.print(Coord_punto_lat[i]-1900,4);Serial.print("N,");Serial.print("99 ");Serial.print(Coord_punto_long[i]-9900,4);Serial.println("W");
  }

  // ============= CALIBRACIÓN GPS =============
  if( calibrado == false ){
    for(int j=0;j<7;j++){                     // 7 iteraciones de calibración
      readGPS();                          // Esto es prácticamente toda la lectura de datos del GPS
      Coord_inicial_lat= GPS.latitude;    // La coordenada inicial del robot tomará la latitud de la función readGPS()
      Coord_inicial_long= GPS.longitude;  // La coordenada inicial del robot tomará la longitud de la función readGPS()  
      Serial.println(NMEA1);
      Serial.println(NMEA2);
      Serial.println("");         
    }
    Serial.println("La calibracion ha terminado, gracias por esperar");
    calibrado = true;
    digitalWrite(LED_calibracion_lista, HIGH);
  }
    
  Serial.print("Las coordenadas iniciales del robot para verlas Google Earth son: ");
  Serial.print("19 ");Serial.print(Coord_inicial_lat-1900,4);Serial.print("N,99 ");Serial.print(Coord_inicial_long-9900,4);Serial.println("W");

  // ============= TRIANGULACIÓN Y COMUNICACIÓN SERIAL =============

  float Latitud_total[n];
  float Longitud_total[n];
  float Thetha_radianes[n];
  float Thetha_grados[n];
      
  //PRIMER ÁNGULO
  Latitud_total[0]= Coord_punto_lat[0]-Coord_inicial_lat;
  Longitud_total[0]= (Coord_punto_long[0]-Coord_inicial_long)*(-1);
  Thetha_radianes[0]= atan(Latitud_total[0]/Longitud_total[0]);
    
  //SIGUIENTES ÁNGULOS
  for(i=1;i<n;i++){
    Latitud_total[i]= Coord_punto_lat[i]-Coord_punto_lat[i-1];
    Longitud_total[i]= (Coord_punto_long[i]-Coord_punto_long[i-1])*(-1);
    Thetha_radianes[i]= atan(Latitud_total[i]/Longitud_total[i]);
  }

  for(i=0;i<n;i++){                                         
    Thetha_grados[i]= Thetha_radianes[i]*RAD_TO_DEG;}         // Transformo los ángulos de dirección a grados

  // ANÁLISIS DE CUADRANTES para que quede siempre en 360°
  for(i=0;i<n;i++){
    // CUADRANTE 1: --------------
    // CUADRANTE 2 y 3:
    if( Longitud_total[i] < 0){
      Thetha_grados[i] = 180+Thetha_grados[i];}
    // CUADRANTE 4:
    if( Latitud_total[i] < 0 && Longitud_total[i] > 0){
      Thetha_grados[i] = 360+Thetha_grados[i];}
  }
   
  Serial.println("");
  Serial.println("El arreglo de angulos de las triangulaciones es:");
  for(i=0;i<n;i++){
    Serial.println(Thetha_grados[i],4);}
    
  for(i=0;i<n;i++){
    Arduinos.print(Thetha_grados[i]);                // Manda el ángulo de dirección "Thetha" al segundo Arduino
    Serial.println("A continuacion viene la seccion de errores relativos:");
    // ============= ERRORES RELATIVOS =============

    while(true){      // No se detendrá hasta llegar a su destino con un error relativo menor al 10%, y se resetearán los dos Arduinos
      readGPS();
      float error_relativo_lat= ( (Coord_punto_lat[i] - GPS.latitude )/Coord_punto_lat[i] )*6.39*pow(10,7);      // 6.39x10^7 es el factor calculado
      float error_relativo_long= ( (Coord_punto_long[i] - GPS.longitude )/Coord_punto_long[i] )*1.1*pow(10,8); // 1.1x10^8 es el factor calculado
      // Para sacar el absoluto del error relativo
      if(error_relativo_lat < 0)
        error_relativo_lat= error_relativo_lat*(-1);
      if(error_relativo_long < 0)
        error_relativo_long= error_relativo_long*(-1);
      Serial.println(error_relativo_lat,4);
      Serial.println(error_relativo_long,4);

      // ============= INTERRUPTOR NEXT ANGLE ==============
      
      // El Robot cambiará de punto objetivo hasta que el error relativo de la latitud y longitud sean menores al 80%
      if( error_relativo_lat < 80 && error_relativo_long < 80 ){
        digitalWrite(interruptor,HIGH);     // Pulso de interrupción
        delay(500);                         // Dejar un delay para que le de tiempo al Segundo Arduino de leer el pulso
        digitalWrite(interruptor,LOW);
        break;
      }   //  END IF
    } // END WHILE(TRUE)
    while(true){
      if( digitalRead(camara_check) == HIGH){
        delay(1500);
        break;                              // Rompe el ciclo while(true)
      }   //    END IF CAMARA CHECK
    }   //    END WHILE(TRUE)
  }   // END FOR
}
// FUNCIONES--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void readGPS() {
  clearGPS();
  while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
  NMEA1=GPS.lastNMEA();
  
   while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
  NMEA2=GPS.lastNMEA();
}
 
void clearGPS() {  //Clear old and corrupt data from serial port 
  while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
  
  while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
   while(!GPS.newNMEAreceived()) { //Loop until you have a good NMEA sentence
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); //Parse that last good NMEA sentence
}
