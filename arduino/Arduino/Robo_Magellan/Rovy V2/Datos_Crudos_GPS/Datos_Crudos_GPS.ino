// ============== BIBLIOTECAS =================
#include <Adafruit_GPS.h> // Biblioteca necesaria para el Ultimate Adafruit GPS Breakout V3
#include <SoftwareSerial.h> // Biblioteca necesaria para multiplexear los puertos seriales del Arduino
//HardwareSerial mySerial = Serial1;
// ======== MULTIPLEXEO DE PUERTOS SERIALES ==========
SoftwareSerial GPSerial(2,3); // (Rx,Tx)
Adafruit_GPS GPS(&GPSerial); // Crea el objeto GPS en base al puerto serial GPSerial
// ========== VARIABLES GLOBALES =================
String NMEA1, NMEA2; // Variables para las sentencias NMEA del GPS
char c; // Variable para leer caracteres del GPS
// ============================================
// ============== VOID SETUP ==================
// ============================================
void setup() {

 // VELOCIDAD DE PUERTOS SERIALES
 Serial.begin(115200); // Serial Monitor
 Serial.println("HELLO");
 GPS.begin(9600); // Serial GPS

 GPS.sendCommand("$PGCMD,33,0*6D"); //Turn off antenna update nuisance data
 GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Request RMC and GGA Sentences only
 GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //Set update rate to 1 hz
}
// ===========================================
// ============== VOID LOOP ==================
// ===========================================
void loop(){
   readGPS(); // Esto es prácticamente toda la lectura de datos del GPS
    Serial.print("s");
    Serial.print(GPS.latitude,4);Serial.print("a");
    Serial.print(GPS.longitude,4);Serial.print("b");
    Serial.print(GPS.fix);Serial.print("c");
    Serial.print(GPS.satellites);Serial.print("d");
    Serial.println("$");
    //Serial.println(NMEA1);
    //Serial.println(NMEA2);
   /* INSNTRUCCIONES COMPLETAS
   Serial.print("Mes:");
   Serial.println(GPS.month);
   Serial.print("Dia:");
   Serial.println(GPS.day);
   Serial.print("Año:");
   Serial.println(GPS.year);
   Serial.print("Fix:");
   Serial.println(GPS.fix);
   Serial.print("Latitud:");
   Serial.println(GPS.latitude);
   Serial.print("Longitud:");
   Serial.println(GPS.longitude);
   Serial.print("Satelites:");
   Serial.println(GPS.satellites);
   Serial.print("Altitud:");
   Serial.println(GPS.altitude);
   Serial.print("Angulo:");
   Serial.println(GPS.angle);
   Serial.print("Velocidad:");
   Serial.println(GPS.speed);
  */
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
