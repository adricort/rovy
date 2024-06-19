// Programa Demo Rovy - RoboMagellan 2018: sirve para saber si todos los sensores actuadores están funcionando correctamente
// PENDIENTES: EL ARDUINO REGRESA "inf" cuando el numero es raro, significa división entre infinito de la IMU. Corregir eso, de vez en cuando lo arroja.

// La numeración de todos los sensores y actuadores se definieron desde la perspectiva del robot, de izquierda a derecha y adelante hacia atrás
/* Valores simétricos máximos, mínimos y de interés:
 *  Servo izquierdo:    Servo derecho:    Camara neck:    Camara head:    PWM RoboClaw:   Encoders:
 *  Retraido: 25°        Retraido: 165°   Izquierda: 5°   Arriba: 100°    Mínimo: 0       D llanta: 0.12 [m]
 *  Extendido: 100°      Extendido: 85°   Centro: 96°     "Centro": 160°  Máximo: 127     1 vuelta enc: 1925 [u] (aproximadamente, +/-10)  
 *                                        Derecha: 180°                                   Formula L recorrida: D*3.1416*1.2* (Lectura_Enc/1925) <--- es el n vueltas
 *
 * NOTA: La función Encoder() lee los datos de los encoders recorrida. Intentar usar cada cierto tiempo, no consecutivamente porque es costoso computacionalmente.
  Coordenadas formato Google Earth para ejemplificar:

  Inicial: 19 18.3253N,99 09.2830W (casa afuera)      Desarrollado por: Ricárdez Ortigosa Adrián
  Final: 19 18.3358N,99 09.2795W (en la coladera de enmedio)
  Final Calle: 19 18.3545N,99 09.2705W
*/                                                                                       

// Bibliotecas
#include <Servo.h>
#include <SoftwareSerial.h> // Biblioteca necesaria para multiplexear los puertos seriales del Arduino
#include "RoboClaw.h"

// Direcciones packet serial RoboClaws
#define address0 0x80
#define address1 0x81
#define address2 0x82

// Multiplexeo de seriales
SoftwareSerial serialUNO(50,52);

// Velocidad de los seriales
RoboClaw roboclaws(&serialUNO,10000);

// Declaración de Motores 99:1 Pololu
Servo Camara_neck;    // Servo direccion cámara
Servo Camara_head;    // Servo base cámara
Servo Bandera_izq;    // Servo base cámara
Servo Bandera_der;    // Servo direccion cámara

// Declaración de pines
const int pin_neck = 26;
const int pin_head = 28;
const int pin_band_izq = 30;
const int pin_band_der = 32;
const int pin_boton1 = 6;
const int pin_boton2 = 7;
const int pin_LED[4] = {2,3,4,5};

// Variables Globales
float lat,lon,fix,satel,IMU_anterior;
boolean primer_dato_IMU = false;    // Bandera para que reciba el primer dato anterior de la IMU sea el primero que recibe, sólo lo hace 1 vez

// Constantes Globales
const int u = 5;                  // Número de ultrasónicos recibidos del Arduino UNO
// Valores simétricos máximos, mínimos y de interés:
const int min_band_izq = 25;
const int max_band_izq = 100;
const int min_band_der = 165;
const int max_band_der = 85;
const int min_neck = 5;
const int cent_neck = 96;
const int max_neck = 180;
const int max_head = 100;
const int cent_head = 160;
const int min_mot = 0;
const int max_mot = 127;
const int una_vuelta_enc = 1925;
const float K_enc = 1.15;
const float D_llanta = 0.12;
const float pi = 3.1416;
// Parse.Int de la cadena
int Coordx_cono;
int Coordy_cono;

// ---------------------------------- FUNCIONES------------------------------------------------
void mot_adel(int vel_m1,int vel_m2,int vel_m3,int vel_m4,int vel_m5,int vel_m6)  // Motores adelante con velocidad
{
    roboclaws.ForwardM1(address0,vel_m1); //start Motor1 forward at half speed
    roboclaws.ForwardM2(address0,vel_m2); //start Motor2 backward at half speed
    roboclaws.ForwardM1(address1,vel_m3); //start Motor1 forward at half speed
    roboclaws.ForwardM2(address1,vel_m4); //start Motor2 backward at half speed
    roboclaws.ForwardM1(address2,vel_m5); //start Motor1 forward at half speed
    roboclaws.ForwardM2(address2,vel_m6); //start Motor2 backward at half speed
}

void mot_atr(int vel_m1,int vel_m2,int vel_m3,int vel_m4,int vel_m5,int vel_m6)   // Motores atrás con velocidad
{
    roboclaws.BackwardM1(address0,vel_m1); //start Motor1 forward at half speed
    roboclaws.BackwardM2(address0,vel_m2); //start Motor2 backward at half speed
    roboclaws.BackwardM1(address1,vel_m3); //start Motor1 forward at half speed
    roboclaws.BackwardM2(address1,vel_m4); //start Motor2 backward at half speed
    roboclaws.BackwardM1(address2,vel_m5); //start Motor1 forward at half speed
    roboclaws.BackwardM2(address2,vel_m6); //start Motor2 backward at half speed
}

void servos(int ang_band_izq, int ang_band_der, int ang_neck, int ang_head)     // Escritura de servos
{
  Bandera_izq.write(ang_band_izq);
  Bandera_der.write(ang_band_der);
  Camara_neck.write(ang_neck);
  Camara_head.write(ang_head);
}

// ====================================== VOID SETUP =================================================================
void setup() {
  Serial.begin(115200);     // Puerto serial para la comunicación con la computadora y datos GPS. Debe estar a 9600 por el GPS.
  Serial2.begin(115200);      // Puerto serial para la recepción de datos de los ultrasónicos e IMU
  Serial1.begin(115200);      // Puerto serial para los datos de la cámara con la Raspberry Pi3

  roboclaws.begin(115200);
}

// ======================================= DATOS ULTRASÓNICOS Y PUERTO SERIAL ARDUINO UNO ================================================================
void leer_US_IMU()
{
  if(Serial2.available()>0){
    String US_IMU = Serial2.readStringUntil(36);     // 36 es el ASCII de $, el fin de la cadena s(NUMERO0)a(NUMERO1)b(NUMERO2)c(NUMERO3)d(NUMERO4)e(NUMERO5)f(NUMERO)6g(NUMERO7)h$
    
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    int indice_0 = US_IMU.lastIndexOf('s');
    int indice_1 = US_IMU.indexOf('a',indice_0+1); //("a",0)
    int indice_2 = US_IMU.indexOf('b',indice_1+1); //("b",indice_1+1)
    int indice_3 = US_IMU.indexOf('c',indice_2+1); //("c",indice_2+1)
    int indice_4 = US_IMU.indexOf('d',indice_3+1); //("d",indice_3+1)
    int indice_5 = US_IMU.indexOf('e',indice_4+1); //("e",indice_4+1)
    int indice_6 = US_IMU.indexOf('f',indice_5+1); //("f",indice_5+1)
    
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = US_IMU.substring(indice_0+1,indice_1);
    String cadena_2 = US_IMU.substring(indice_1+1,indice_2);
    String cadena_3 = US_IMU.substring(indice_2+1,indice_3);
    String cadena_4 = US_IMU.substring(indice_3+1,indice_4);
    String cadena_5 = US_IMU.substring(indice_4+1,indice_5);
    String cadena_6 = US_IMU.substring(indice_5+1,indice_6);

    // Parse.Int o Float de la cadena
    int uS_1 = cadena_1.toInt();
    int uS_2 = cadena_2.toInt();
    int uS_3 = cadena_3.toInt();
    int uS_4 = cadena_4.toInt();
    int uS_5 = cadena_5.toInt();
    float IMU = (360 - cadena_6.toFloat());   // 360 - el ángulo porque curiosamente crece al revés xD
    if(!primer_dato_IMU)
    {
      IMU_anterior = IMU; // Actualiza el valor
      primer_dato_IMU = true;
    }

    // El siguiente condicional sirve para filtrar el ruido que genera tener combinados los US (int) y la IMU (float)
    if(uS_1 <= 1 && uS_2 <= 1 && uS_3 <= 1 && uS_4 <= 1 && uS_5 <= 1 && IMU != 0 && (abs(IMU-IMU_anterior) < 10) && IMU != 0 ) 
    { 
      IMU_anterior = IMU; // Actualiza el valor
       
       // Despliegue de vflores de los ultrasónicos
      Serial.print("US1: ");
      Serial.print(uS_1);
      Serial.print(" US2: ");
      Serial.print(uS_2);
      Serial.print(" US3: ");
      Serial.print(uS_3);
      Serial.print(" US4: ");
      Serial.print(uS_4);
      Serial.print(" US5: ");
      Serial.print(uS_5);
      Serial.print(" IMU: ");
      Serial.println(IMU);
    }

  }
}

float leerGPS()
{
  if(Serial.available()>0){
    String GPS = Serial.readStringUntil(36);     // 36 es el ASCII de $, el fin de la cadena s(NUMERO0)a(NUMERO1)b(NUMERO2)c(NUMERO3)d(NUMERO4)$
    
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    float indice_0 = GPS.lastIndexOf('s');
    float indice_1 = GPS.indexOf('a',indice_0+1); //("a",0)
    float indice_2 = GPS.indexOf('b',indice_1+1); //("b",indice_1+1)
    float indice_3 = GPS.indexOf('c',indice_2+1); //("c",indice_2+1)
    float indice_4 = GPS.indexOf('d',indice_3+1); //("d",indice_3+1)
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = GPS.substring(indice_0+1,indice_1);
    String cadena_2 = GPS.substring(indice_1+1,indice_2);
    String cadena_3 = GPS.substring(indice_2+1,indice_3);
    String cadena_4 = GPS.substring(indice_3+1,indice_4);
    
    // Parse.Int de la cadena
     lat = cadena_1.toFloat();
     lon = cadena_2.toFloat();
     fix = cadena_3.toFloat();
     satel = cadena_4.toFloat();

     /*Serial.print("lat: ");Serial.print(lat,4);
     Serial.print(" long: ");Serial.print(lon,4);
     Serial.print(" fix: ");Serial.print(fix);
     Serial.print(" satel: ");Serial.println(satel);*/
  }
}

void leerCamara(void)
{
  if(Serial1.available()){
    String leerCamara = Serial1.readStringUntil(122);     // 122 es el ASCII de z, el fin de la cadena s(Coordx)a(Coordy)bz, en la rasp no poner letras raras   
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    int indice_0 = leerCamara.lastIndexOf('s');
    int indice_1 = leerCamara.indexOf('a',indice_0+1); //("a",0)
    int indice_2 = leerCamara.indexOf('b',indice_1+1); //("b",indice_1+1)
     
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = leerCamara.substring(indice_0+1,indice_1);
    String cadena_2 = leerCamara.substring(indice_1+1,indice_2);

    // Parse.Int de la cadena
    Coordx_cono = cadena_1.toInt();
    Coordy_cono = cadena_2.toInt();

    // Despliegue de vflores de los ultrasónicos
    Serial.print("Coordx_cono: ");
    Serial.print(Coordx_cono);
    Serial.print("  Coordy_cono: ");
    Serial.println(Coordy_cono);
  }
}

void Encoders()
{
  uint8_t status_enc[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  bool valid_enc[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  int32_t enc[6]={0,0,0,0,0,0};
  int32_t speeds[6]={0,0,0,0,0,0};
  float L_enc[6]={0,0,0,0,0,0};

  // RoboClaw1 (adelante)
  enc[0] = roboclaws.ReadEncM1(address0, &status_enc[0], &valid_enc[0]);
  enc[1] = roboclaws.ReadEncM2(address0, &status_enc[1], &valid_enc[1]);
  speeds[0] = roboclaws.ReadSpeedM1(address0, &status_enc[2], &valid_enc[2]);
  speeds[1] = roboclaws.ReadSpeedM2(address0, &status_enc[3], &valid_enc[3]);

  // RoboClaw2 (centro)
  enc[2] = roboclaws.ReadEncM1(address1, &status_enc[4], &valid_enc[4]);
  enc[3] = roboclaws.ReadEncM2(address1, &status_enc[5], &valid_enc[5]);
  speeds[2] = roboclaws.ReadSpeedM1(address1, &status_enc[6], &valid_enc[6]);
  speeds[3] = roboclaws.ReadSpeedM2(address1, &status_enc[7], &valid_enc[7]);

  // RoboClaw3 (atrás)
  enc[4] = roboclaws.ReadEncM1(address2, &status_enc[8], &valid_enc[8]);
  enc[5] = roboclaws.ReadEncM2(address2, &status_enc[9], &valid_enc[9]);
  speeds[4] = roboclaws.ReadSpeedM1(address2, &status_enc[10], &valid_enc[10]);
  speeds[5] = roboclaws.ReadSpeedM2(address2, &status_enc[11], &valid_enc[11]);

  for(int j = 0;j < 6; j++)
  {
    Serial.print("[m]Enc");Serial.print(j);Serial.print(" ");
    if(valid_enc[j])
    {
      L_enc[j] = D_llanta*pi*K_enc*(enc[j]/una_vuelta_enc);     
      Serial.print(L_enc[j],4);
      Serial.print(" ");
    }
    else
      Serial.print("FAIL ");
  }
  Serial.println();
}

void LEDs(boolean verde, boolean amarillo, boolean azul, boolean rojo)
{
    digitalWrite(pin_LED[0],verde);
    digitalWrite(pin_LED[1],amarillo);
    digitalWrite(pin_LED[2],azul);
    digitalWrite(pin_LED[3],rojo);
}

// ===================================================== VOID LOOP ============================================================
void loop(){
    
    Serial.println("MANDA CUALQUIER CARACTER PARA COMENZAR EL DEMO");
    Serial.println("");
    while(Serial.available()==0){}    // Espera hasta que ingrese un dato en el monitor serial para comenzar el demo
    Serial.println("INICIANDO RECOLECCION DE DATOS...");
    
    for(int i = 0; i < 100; i++)
      leer_US_IMU();

    Serial.println("INICIANDO RUTINA DE ACTUADORES...");
    
    // Rutina de prueba de todos los motores adelante
    Serial.println("MOTOR 1 ADELANTE");
    for(int i = min_mot; i <= (max_mot+1); i++)
      mot_adel(50,0,0,0,0,0);
    Serial.println("MOTOR 2 ADELANTE");
    for(int i = min_mot; i <= (max_mot+1); i++)
      mot_adel(0,50,0,0,0,0);
    Serial.println("MOTOR 3 ADELANTE");
    for(int i = min_mot; i <= (max_mot+1); i++)
      mot_adel(0,0,50,0,0,0);
    Serial.println("MOTOR 4 ADELANTE");
    for(int i = min_mot; i <= (max_mot+1); i++)
      mot_adel(0,0,0,50,0,0);
    Serial.println("MOTOR 5 ADELANTE");
    for(int i = min_mot; i <= (max_mot+1); i++)
      mot_adel(0,0,0,0,50,0);
    Serial.println("MOTOR 6 ADELANTE");
    for(int i = min_mot; i <= (max_mot+1); i++)
      mot_adel(0,0,0,0,0,50);

    Serial.println("MOTORES DETENIDOS");
    mot_adel(0,0,0,0,0,0);
    delay(500);

    Serial.println("LEYENDO ENCODERS...");
    Encoders();

    Serial.println("MOTORES RETROCEDIENDO");
    // Rutina de prueba de todos los motores atras
    for(int i = min_mot; i <= (max_mot+1); i++)
      mot_atr(50,50,50,50,50,50);

    Serial.println("MOTORES DETENIDOS");
    mot_adel(0,0,0,0,0,0);
    delay(500);
    

    Serial.println("LEYENDO ENCODERS REGRESO...");
    Encoders();
    delay(1000);

    Serial.println();
    Serial.println("Leyendo camara:");
    do
      leerCamara();
    while(Coordx_cono == 0 || Coordy_cono == 0);

    Serial.println("RUTINA DE SERVOS:");
    // Rutina de servos
    Camara_neck.attach(pin_neck);
    Camara_head.attach(pin_head);
    Bandera_izq.attach(pin_band_izq);
    Bandera_der.attach(pin_band_der);
    Camara_neck.write(cent_neck);
    Camara_head.write(cent_head);
    Bandera_izq.write(min_band_izq);
    Bandera_der.write(min_band_der);
    delay(3000);
    
    servos(max_band_izq,min_band_der,cent_neck,cent_head);
    delay(1000);
    servos(min_band_izq,min_band_der,cent_neck,cent_head);
    delay(1000);

    servos(min_band_izq,max_band_der,cent_neck,cent_head);
    delay(1000);
    servos(min_band_izq,min_band_der,cent_neck,cent_head);
    delay(1000);
    
    servos(min_band_izq,min_band_der,min_neck,cent_head);
    delay(500);
    servos(min_band_izq,min_band_der,cent_neck,cent_head);
    delay(500);
    servos(min_band_izq,min_band_der,max_neck,cent_head);
    delay(500);
    
    servos(min_band_izq,min_band_der,cent_neck,cent_head);
    delay(500);
    servos(min_band_izq,min_band_der,cent_neck,max_head);
    delay(500);
    
    servos(min_band_izq,min_band_der,cent_neck,cent_head);
    delay(500);

    Camara_neck.detach();
    Camara_head.detach();
    Bandera_izq.detach();
    Bandera_der.detach();

    Serial.println();
    Serial.println("DESCONECTE CABLE USB-LAPTOP Y CONECTE RX/TX DEL GPS PARA PROBAR PANEL");
    Serial.println("LED rojo = !fix, LED Azul = fix, LED amarillo = boton1, LED verde = boton2");
    delay(1000);
    Serial.println("Listo para desconectarse...");
    
    // LECTURA DEL PANEL
    while(true)
    {
      leerGPS();
      LEDs(digitalRead(pin_boton1),digitalRead(pin_boton2),fix,!fix);
    }
}
