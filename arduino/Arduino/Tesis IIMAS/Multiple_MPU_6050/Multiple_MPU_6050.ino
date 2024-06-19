/* INSTITUTO DE INVESTIGACIONES EN MATEMÁTICAS Y SISTEMAS - EQUIPO:       NOMBRE_DEL_EQUIPO   (ejemplo: SPARTAN CROFI)
                                                            PROGRAMA:     NOMBRE_DEL_PROGRAMA (ejemplo: SegundoArduino_ROVY_2_puntos)
                                                            ROBOT:        NOMBRE_DEL_ROBOT    (ejemplo: ROVY)
                                                            CATEGORÍA:    NOMBRE_CATEGORÍA    (ejemplo: ROBO MAGELLAN 
                                                            PROGRAMADOR:  NOMBRE_PROGRAMADOR  (ejemplo: Adrián Ricárdez O.)     
DESCRIPCIÓN Y COMENTARIOS
Descripción_breve_para_qué_sirve_el_programa
(ejemplo: Este programa sirve para orientar al robot rover en la dirección correcta debido a los ángulos de dirección dados por
          el PrimerArduino con GPS, coordina la etapa de potencia en PWM, y para la orientación utiliza una IMU
          Advertencia: Utilizar de preferencia WatchDog Timer para salirse de bugs [loops infinitos])

HARDWARE
Lista_de_Hardware_utilizado_y_modelo
(ejemplo: - 5 IMU's MPU-6050
          - 1 Arduino UNO
          - Jumpers

CONEXIONES
Pines_de_conexiones
(ejemplo: Segundo Arduino        MPU-6050
          A4          -->          SDA
          A5          -->          SCL
          +5V         -->          Vin
          GND         -->          GND
          
          Segundo Arduino    Primer Arduino
          pin 7          -->   pin 6
          pin 6          -->   pin 7
          GND            -->   GND

          Segundo Arduino     Batería +9V
          Vin            -->     +9V  
          GND            -->     -9V

VARIABLES DE TEÓRICAS/EXPERIMENTALES
(ejemplo: Valores de PWM para los motores:
Adelante: 197-253   Donde el 197 es el más lento y el 253 es el más rápido
Atrás:    1-185     Donde el 185 es el más lento y el 1 es el más rápido
Paro:     186-196   El promedio de PARO es 191)
*/

// ------------------------------------ BIBLIOTECAS -----------------------------------------------------
#include "Wire.h"     // Finalidad de la biblioteca
#include "I2Cdev.h"
#include "MPU6050.h"
#include <DS3231.h>
#include <SD.h>b

// ------------------------------------ OBJETOS ---------------------------------------------------------
MPU6050 accelgyro(0x69);
DS3231  rtc(SDA, SCL);

// ------------------------------------ DECLARACIÓN DE PINES --------------------------------------------
const int LED = 13;
const int AD0[5] = {2, 3, 4, 5, 6};   // Pines que activarán el AD0
// ----------------------------------- VARIABLES GLOBALES -----------------------------------------------
unsigned long time;
int16_t ax, ay, az, ax1, ay1, az1, ax2, ay2, az2, ax3, ay3, az3, ax4, ay4, az4, ax5, ay5, az5;    // Aceleraciones
int16_t gx, gy, gz, gx1, gy1, gz1, gx2, gy2, gz2, gx3, gy3, gz3, gx4, gy4, gz4, gx5, gy5, gz5;    // Vel angulares

bool blinkState = false;

File myFile;

// ==========================================================================================
// ======================================= VOID SETUP =======================================
// ==========================================================================================

void setup()
{
  
  Serial.begin(115200);  // Monitor Serial (baudrate)
  Serial.print("Iniciando SD ... ");
  
  Wire.begin();         // Inicializa el puerto I2C
  rtc.begin();

  if (!SD.begin(4)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("Inicializacion exitosa");
  
  // configure Arduino LED for
  pinMode(LED, OUTPUT);
  // Declaración de salidas e inicialización de cada una de las IMU's
  for(int i = 0; i < 5; i++)
  {
    pinMode(AD0[i],OUTPUT);
    digitalWrite(AD0[i],HIGH);  // Lo pone en dirección 0x69
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();     // Inicializa al IMU[i]
    digitalWrite(AD0[i],LOW);   // Se apaga para que ninguna tenga la dirección 0x69
  }

  if(!SD.exists("datalog.csv"))
  {
    Serial.println("Archivo nuevo, Escribiendo encabezado(fila 1)");
        myFile.println("Tiempo(ms),ax,ay,az,gx,gy,gz");
        myFile.close();
  }

  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  //delay(3000);
}

// ==========================================================================================
// ======================================= VOID LOOP ========================================
// ==========================================================================================
void loop() 
{
  myFile = SD.open("BIPEDO.csv",FILE_WRITE);//abrimos  el archivo
  if (myFile) { 
    Serial.print("Escribiendo SD: ");
    myFile.print("a[x y z] g[x y z]:\t");
    
    for(int i = 0; i < 5; i++)
    {
      digitalWrite(AD0[i],HIGH);  // Lo pone en dirección 0x69
      //accelgyro.initialize();
      
      // read raw accel/gyro measurements from device
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz );
      
      // display tab-separated accel/gyro x/y/z values
      //Serial.print("MPU:\t");
      Serial.print(ax); Serial.print("\t");
      Serial.print(ay); Serial.print("\t");
      Serial.print(az); Serial.print("\t");
 
      myFile.print(",");
      myFile.print(ax);
      myFile.print(",");
      myFile.print(ay);
      myFile.print(",");
      myFile.print(az); 
      myFile.print(",");
      myFile.print(gx); 
      myFile.print(",");
      myFile.print(gy);
      myFile.print(",");
      myFile.print(gz);
      myFile.print(",");
      
      /*Serial.print(gx); Serial.print("\t");
      Serial.print(gy); Serial.print("\t");
      Serial.print(gz);*/
    
      digitalWrite(AD0[i],LOW);
    }
    digitalWrite(AD0[0],HIGH);digitalWrite(AD0[1],HIGH);digitalWrite(AD0[2],HIGH);digitalWrite(AD0[3],HIGH);digitalWrite(AD0[4],HIGH);  // Se ponen en dirección 0x69 a los MPU
    // Send Day-of-Week
    Serial.print(rtc.getDOWStr());
    Serial.print(" ");
    myFile.print(rtc.getDOWStr());
    myFile.print(" ");

    // Send date
    Serial.print(rtc.getDateStr());
    Serial.print(" -- ");
    myFile.print(rtc.getDateStr());
    myFile.print(" -- ");

    // Send time
    Serial.println(rtc.getTimeStr());
    myFile.println(rtc.getTimeStr());
    digitalWrite(AD0[0],LOW);digitalWrite(AD0[1],LOW);digitalWrite(AD0[2],LOW);digitalWrite(AD0[3],LOW);digitalWrite(AD0[4],LOW); // Se ponen en dirección 0x68 a los MPU
  
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED, blinkState);
    myFile.close();
  }
}
