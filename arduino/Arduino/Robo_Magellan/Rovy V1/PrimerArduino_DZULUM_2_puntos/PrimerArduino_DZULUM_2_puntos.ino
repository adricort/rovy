/* CLUB DE ROBÓTICA DE LA FACULTAD DE INGENIERÍA- Equipo: SPARTAN CROFI
                                                                          PROGRAMA DEL PRIMER ARDUINO DZULÚM (ROVY). CATEGORÍA: ROBO MAGELLAN
                                                                                            SECCIÓN DE DATOS
Coordenadas Google Earth para ejemplificar: 
Inicial: 19 18.3253N,99 09.2845W  (casa afuera)                                               Desarrollado por: Ricárdez Ortigosa Adrián
Final: 19 18.3358N,99 09.2795W    (en la coladera de enmedio)

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
pin 8          -->   Reset
*/

// BIBLIOTECAS
// VARIABLES GLOBALES
#include <Adafruit_GPS.h>         // Biblioteca necesaria para el Ultimate Adafruit GPS Breakout V3
String NMEA1, NMEA2;              // Variables para las sentencias NMEA del GPS
char c;                           // Variable para leer caracteres del GPS
float deg;                        // Soportará el funcionamiento del Ultimate Adafruit GPS Breakout V3
#include <SoftwareSerial.h>       // Biblioteca necesaria para multiplexear los puertos seriales del Arduino

// MULTIPLEXEO DE PUERTOS SERIALES
SoftwareSerial GPSerial(4,5);     // (Rx,Tx)
SoftwareSerial Arduinos(6,7);     // (Rx,Tx)

Adafruit_GPS GPS(&GPSerial);      // Crea el objeto GPS en base al puerto serial GPSerial

const int reset = 8;               // Este pin reseteara ambos datos de la posición en formato de ángulo simple
float degWhole;                    // Variable para toda la parte de posición
float degDec;                      // Variable para la parte decimal de ángulo
float Coord_inicial_lat;           // Coordenada inicial del robot en latitud
float Coord_inicial_long;          // Coordenada inicial del robot en latitud

// ============================================
// ============== VOID SETUP ==================
// ============================================
void setup() {
  digitalWrite(reset, HIGH);
  pinMode(reset, OUTPUT);
  
  // VELOCIDAD DE PUERTOS SERIALES
  Serial.begin(9600);   // Serial Monitor
  Arduinos.begin(9600); // Serial Arduinos
  GPS.begin(9600);      // Serial GPS
  
  GPS.sendCommand("$PGCMD,33,0*6D");            //Turn off antenna update nuisance data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Request RMC and GGA Sentences only
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);    //Set update rate to 1 hz
  delay(10);
}

// ===========================================
// ============== VOID LOOP ==================
// ===========================================
void loop(){
  
  Serial.print("Ingrese la latitud final en grados y minutos juntos, ejemplo '1918.3358':  ");
  while (Serial.available()==0){                                                // Espera la entrada del usuario  
  }
  float Coord_final_lat= Serial.parseFloat();
  Serial.println(Coord_final_lat,4);
  delay(500);
  Serial.print("Ingrese la longitud final en grados y minutos juntos, ejemplo '9909.2795':  ");
  while (Serial.available()==0){                                                // Espera la entrada del usuario  
  }
  float Coord_final_long= Serial.parseFloat();
  Serial.println(Coord_final_long,4);
  delay(500);
  Serial.print("Copiar y pegar en Google Earth las siguientes coordenadas para poder ver la ubicacion:  ");
  Serial.print("19 ");Serial.print(Coord_final_lat-1900,4);Serial.print("N,");Serial.print("99 ");Serial.print(Coord_final_long-9900,4);Serial.println("W");Serial.println("");
  delay(1000);
  
  Serial.println("A continuacion, viene la calibracion del GPS, espere un momento porfavor...");
  for(int i=0;i<30;i++){    // N iteraciones de calibración
    readGPS();
    Coord_inicial_lat= GPS.latitude;   // Ejemplo: 1918.3253
    Coord_inicial_long= GPS.longitude; // Ejemplo: 9909.2845
    Serial.println(NMEA1);
    Serial.println(NMEA2);
    Serial.println("");           
  }
  
  Serial.println("La calibracion ha terminado, gracias por esperar");
  //delay(500);

  Serial.print("Las coordenadas de la posicion inicial del robot para verlas Google Earth son: ");
  Serial.print("19 ");Serial.print(Coord_inicial_lat-1900,4);Serial.print("N,99 ");Serial.print(Coord_inicial_long-9900,4);Serial.println("W");
  //delay(1000);

  float latitud_total= Coord_final_lat - Coord_inicial_lat;     // Ejemplo: 1918.3358 - 1918.3253 = 0.0105
  float longitud_total= (Coord_final_long - Coord_inicial_long)*(-1);  // Ejemplo: (9909.2795 - 9909.2845)*(-1)= 0.005, lo multiplico por -1 porque el eje de las "x" (longitud) en Google Earth está al revés porque es "West"

  Serial.print("La latitud total de su triangulo (cateto opuesto) es de: ");Serial.println(latitud_total,5);
  Serial.print("La longitud total de su triangulo (cateto adyacente) es de: ");Serial.println(longitud_total,5);
  //delay(1000);

  float thetha_radianes= atan(latitud_total/longitud_total);       // Ejemplo: atan(0.0105/0.005)= 1.1264
  float thetha_grados= thetha_radianes*RAD_TO_DEG;              // Ejemplo: 1.1264*(180/pi)= 64.5379°
  // Análisis de cuadrantes para que quede siempre en 360°
  if(longitud_total < 0)
    thetha_grados = 180+thetha_grados;
  if(latitud_total < 0 && longitud_total > 0)
    thetha_grados = 360+thetha_grados;
  Serial.print("El angulo de direccion en radianes es de:  ");
  Serial.println(thetha_radianes,5);                           // Ejemplo: 1.1264
  Serial.print("El angulo de direccion en grados es de:  "); 
  Serial.print(thetha_grados,5);Serial.println("  grados");  // Ejemplo: 64.5379°
  
  Arduinos.print(thetha_grados);                // Manda el ángulo de dirección "thetha" al segundo Arduino

  while(true){      // No se detendrá hasta llegar a su destino con un error relativo menor al 10%, y se resetearán los dos Arduinos
    readGPS();
    float error_relativo_lat= ( (Coord_final_lat - GPS.latitude )/Coord_final_lat )*6.39*pow(10,7);      // 6.39x10^7 es el factor calculado
    float error_relativo_long= ( (Coord_final_long - GPS.longitude )/Coord_final_long )*1.1*pow(10,8); // 1.1x10^8 es el factor calculado
    // Para sacar el absoluto del error relativo
    if(error_relativo_lat < 0)
      error_relativo_lat= error_relativo_lat*(-1);
    if(error_relativo_long < 0)
      error_relativo_long= error_relativo_long*(-1);
    Serial.print("El error relativo de la latitud actual del robot con respecto a la latitud deseada es de: ");
    Serial.print(error_relativo_lat,8);Serial.println(" % ");
    Serial.print("El error relativo de la longitud actual del robot con respecto a la longitud deseada es de: ");
    Serial.print(error_relativo_long,8);Serial.println(" % ");

    // El Robot se reseteará hasta que el error relativo de la latitud y longitud sean menores al 40%
    if( error_relativo_lat < 40 && error_relativo_long < 40 )
      digitalWrite(reset,LOW);     // Para que se reseteén los dos Arduinos
  }
  
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
