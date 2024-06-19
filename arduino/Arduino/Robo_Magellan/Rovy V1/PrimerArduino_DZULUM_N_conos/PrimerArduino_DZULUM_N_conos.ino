/* CLUB DE ROBÓTICA DE LA FACULTAD DE INGENIERÍA- Equipo: SPARTAN CROFI
PROGRAMA DEL PRIMER ARDUINO DZULÚM (ROVY).
CATEGORÍA: ROBO MAGELLAN
SECCIÓN DE DATOS
Coordenadas formato Google Earth para ejemplificar:

Inicial: 19 18.325N,99 09.283W (casa afuera)      Desarrollado por: Ricárdez Ortigosa Adrián y Ayala Almeda Alejandro
Final: 19 18.336N,99 09.279W (en la coladera de enmedio)
Final Calle: 19 18.355N,99 09.271W
*/

// ============== BIBLIOTECAS =================
#include <Adafruit_GPS.h>   // Biblioteca necesaria para el Ultimate Adafruit GPS Breakout V3
#include <SoftwareSerial.h> // Biblioteca necesaria para multiplexear los puertos seriales del Arduino
// ======== MULTIPLEXEO DE PUERTOS SERIALES ==========
SoftwareSerial GPSerial(4,5); // (Rx,Tx)
SoftwareSerial Arduinos(6,7); // (Rx,Tx)
Adafruit_GPS GPS(&GPSerial);  // Crea el objeto GPS en base al puerto serial GPSerial
// ========== VARIABLES GLOBALES =================
String NMEA1, NMEA2;        // Variables para las sentencias NMEA del GPS
char c;                     // Variable para leer caracteres del GPS
const int LED_calibracion_lista = 10; // Pin que indicará que la calibración del GPS ya se hizo
const int pulso_error_rel= 8;       // Pin que generará el pulso al Segundo Arduino, de que ya llegó al 80% de error relativo el GPS
const int pulso_camara_check = 11;  // Pin que recibirá el pulso de terminación de la cámara del Segundo Arduino
float degWhole;               // Variable para toda la parte de posición
float degDec;                 // Variable para la parte decimal de ángulo
float Coord_inicial_lat;      // Coordenada inicial del robot en latitud
float Coord_inicial_long;     // Coordenada inicial del robot en longitud
float deg;                    // Soportará el funcionamiento del Ultimate Adafruit GPS Breakout V3
boolean calibrado = false;    // Variable booleana que determina si ya se calibró una vez el GPS
int i;                        // Para for's
// ============================================
// ============== VOID SETUP ==================
// ============================================
void setup() {

 // VELOCIDAD DE PUERTOS SERIALES
 Serial.begin(9600); // Serial Monitor
 Arduinos.begin(9600); // Serial Arduinos
 GPS.begin(9600); // Serial GPS
 pinMode(pulso_error_rel,OUTPUT);
 pinMode(LED_calibracion_lista,OUTPUT);
 pinMode(pulso_camara_check,INPUT);
 digitalWrite(pulso_error_rel,LOW);

 GPS.sendCommand("$PGCMD,33,0*6D"); //Turn off antenna update nuisance data
 GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Request RMC and GGA Sentences only
 GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //Set update rate to 1 hz
 delay(10);
}
// ===========================================
// ============== VOID LOOP ==================
// ===========================================
void loop(){

// ============= RECEPCIÓN DE DATOS =============
  Serial.println("PROGRAMA PRIMER ARDUINO DZULUM (ROVY)- CATEGORIA ROBO MAGELLAN- CLUB DE ROBOTICA DE LA FACULTAD DE INGENIERIA (CROFI)");Serial.println("");

  Serial.print("Ingrese la cantidad de puntos objetivo del robot, ejemplo '5': ");
  while(Serial.available()==0){} // Espera la entrada de la cantidad de puntos objetivo del usuario "n"
  int n = Serial.parseInt();     // La variable tomará el valor sólo de la parte de número entero que se ingrese
  Serial.println(n);
  float Coord_punto_lat[n];  // Crea el arreglo de coordenadas de los puntos objetivo en latitud
  float Coord_punto_long[n]; // Crea el arreglo de coordenadas de los puntos objetivo en longitud
  for(i=0;i<n;i++){
    Serial.print("Ingrese la latitud de la coordenada numero ");Serial.print(i+1);Serial.print(" en dato WGS84, ejemplo'1918.3358': ");
    while(Serial.available()==0){} // Espera la entrada de la cantidad de puntos objetivo del usuario
    Coord_punto_lat[i]= Serial.parseFloat(); // La variable tomará el valor sólo de la parte de número flotante que se ingrese
    Serial.println(Coord_punto_lat[i],4);

    Serial.print("Ingrese la longitud de la coordenada numero ");Serial.print(i+1);Serial.print(" en dato WGS84, ejemplo '9909.2795': ");
    while(Serial.available()==0){}
    Coord_punto_long[i]= Serial.parseFloat(); // La variable tomará el valor sólo de la parte de número flotante que se ingrese
    Serial.println(Coord_punto_long[i],4);
  }
  Serial.println("El arreglo de coordenadas objetivo en formato Google Earth queda como:");
  for(i=0;i<n;i++){
    Serial.print("19 ");Serial.print(Coord_punto_lat[i]-1900,4);Serial.print("N,");Serial.print("99 ");Serial.print(Coord_punto_long[i]-9900,4);Serial.println("W");
  }
// ============= CALIBRACIÓN GPS =============
  if( calibrado == false ){
    for(int j=0;j<8;j++){   // 8 iteraciones de calibración
    readGPS();              // Esto es prácticamente toda la lectura de datos del GPS
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

  // PRIMER ÁNGULO
  Latitud_total[0]= Coord_punto_lat[0]-Coord_inicial_lat;
  Longitud_total[0]= (Coord_punto_long[0]-Coord_inicial_long)*(-1);
  Thetha_radianes[0]= atan(Latitud_total[0]/Longitud_total[0]);

  // SIGUIENTES ÁNGULOS
  for(i=1;i<n;i++){
    Latitud_total[i]= Coord_punto_lat[i]-Coord_punto_lat[i-1];
    Longitud_total[i]= (Coord_punto_long[i]-Coord_punto_long[i-1])*(-1);
    Thetha_radianes[i]= atan(Latitud_total[i]/Longitud_total[i]);
  }
  for(i=0;i<n;i++){
    Thetha_grados[i]= Thetha_radianes[i]*RAD_TO_DEG;}   // Transformo los ángulos de dirección a grados
  // ANÁLISIS DE CUADRANTES para que quede siempre en 360° el angulo tangente
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
  Serial.println("El arreglo de angulos de las triangulaciones de 0 a 360 es:");
  for(i=0;i<n;i++){
    Serial.println(Thetha_grados[i],4);}

  for(i=0;i<n;i++){
    Arduinos.print(Thetha_grados[i]);     // Manda el ángulo de dirección "Thetha" al segundo Arduino
    Serial.println("A continuacion viene la seccion de errores relativos:");

// ============= ERRORES RELATIVOS E INICIO DE CAMARA=============
    while(true){
      readGPS();
      float error_relativo_lat= ( (Coord_punto_lat[i] - GPS.latitude )/Coord_punto_lat[i])*6.39*pow(10,7); // 6.39x10^7 es el factor calculado
      float error_relativo_long= ( (Coord_punto_long[i] - GPS.longitude )/Coord_punto_long[i])*1.1*pow(10,8); // 1.1x10^8 es el factor calculado
      // Para sacar el absoluto del error relativo
      if(error_relativo_lat < 0){
        error_relativo_lat= error_relativo_lat*(-1);}
      if(error_relativo_long < 0){ 
        error_relativo_long= error_relativo_long*(-1);}
      Serial.print("Error relativo latitud: ");Serial.println(error_relativo_lat,4);
      Serial.print("Error relativo longitud:  ");Serial.println(error_relativo_long,4);
    
      // El Robot cambiará de punto objetivo hasta que el error relativo de la latitud y longitud sean menores al 80%
      if( error_relativo_lat < 80 && error_relativo_long < 80 ){
        digitalWrite(pulso_error_rel,HIGH);    // Pulso para el Segundo Arduino, para que inicie la cámara
        while(digitalRead(pulso_camara_check) == LOW){}    // Mientras la cámara no haya terminado de buscar conos, no hará nada este Arduino
        // Una vez salido del While:
        digitalWrite(pulso_error_rel,LOW);      // Reinicializa el estado del pulso de error relativo
        delay(500);                             // Se espera medio segundo para poder mandarle el siguiente ángulo al Segundo Arduino
        break;                                  // Rompe el ciclo while(true)
      } // FIN IF
    } // FIN WHILE(TRUE)
  }  // FIN FOR
} // FIN LOOP

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
void clearGPS() { //Clear old and corrupt data from serial port
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
