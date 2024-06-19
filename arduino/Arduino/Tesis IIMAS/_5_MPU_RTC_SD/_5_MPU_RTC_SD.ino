// INSTITUTO DE INVESTIGACIONES EN MATEMÁTICAS Y SISTEMAS
// Programa realizado por: Adrián Ricárdez O. 55 60 63 63 81 CDMX

// ------------------------------------ BIBLIOTECAS -----------------------------------------------------
#include "Wire.h"     // Finalidad de la biblioteca
#include "I2Cdev.h"
#include "MPU6050.h"
#include <DS3231.h>
#include <SD.h>

// ------------------------------------ OBJETOS ---------------------------------------------------------
MPU6050 accelgyro(0x69);
DS3231  rtc(SDA, SCL);

// ------------------------------------ DECLARACIÓN DE PINES --------------------------------------------
const int LED = 13;       // LED indicador
const int button = 12;    // Botón de inicialización
const int CS_PIN = 53;    // Este es el pin en el MEGA, en el UNO es el 4 o el 10
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

  // configure Arduino LED for
  pinMode(LED, OUTPUT);
  pinMode(button, INPUT);
  pinMode(CS_PIN, OUTPUT);
  
  Wire.begin();         // Inicializa el puerto I2C
  rtc.begin();
  

  if (!SD.begin(CS_PIN)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("Inicializacion exitosa");
  
  // Declaración de salidas e inicialización de cada una de las IMU's
  for(int i = 0; i < 5; i++)
  {
    pinMode(AD0[i],OUTPUT);     // Declara los cambios de dirección como salidas
    digitalWrite(AD0[i],HIGH);  // Lo pone en dirección 0x69
    
    Serial.println("Inicializacion de dispositivos MPU6050...");
    accelgyro.initialize();     // Inicializa al IMU[i]
    digitalWrite(AD0[i],LOW);   // Se apaga para que ninguna tenga la dirección 0x69
  }

  if(!SD.exists("datalog.csv"))
  {
    Serial.print("Archivo nuevo creado. ");
    Serial.println("Escribiendo en SD. ");
    // Formato en el que se escribirán para poder separar por comas los datos:
    Serial.println("angx1,angy1,angz1,angx2,angy2,angz2,angx3,angy3,angz3,angx4,angy4,angz4,angx5,angy5,angz5,t \n");
    myFile.println("angx1,angy1,angz1,angx2,angy2,angz2,angx3,angy3,angz3,angx4,angy4,angz4,angx5,angy5,angz5,t \n");
  }

  // --- Descomentar lo siguiente para comprobar conexión con los dispositivos I2C ---
  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  while(!digitalRead(button)){} // Espera a que el usuario presione el botón
  delay(3000);                  // Para que se posicione el usuario
  digitalWrite(LED,HIGH);       // Indicador de que está registrando datos

  // Configuración del RTC
  //rtc.setDOW(MONDAY);      // Configuración día de la semana
  //rtc.setTime(15, 29, 0);     // Configuración hora
  //rtc.setDate(4, 12, 2017);   //  Configuración fecha
}

// ==========================================================================================
// ======================================= VOID LOOP ========================================
// ==========================================================================================
void loop() 
{
  while(true){
    myFile = SD.open("BIPEDO.csv",FILE_WRITE);  // Abrimos el archivo llamado "BIPEDO.csv" de tipo escritura
    if(myFile){ 
      //if(digitalRead(button)) // Si presiona el botón
        //break;
         
      for(int i = 0; i < 5; i++)
      {
        digitalWrite(AD0[i],HIGH);  // Lo pone en dirección 0x69
        //accelgyro.initialize();
        
        // Lee las aceleraciones lineales y velocidades angulares de la IMU
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz );
  
        // Escritura de los datos:
        Serial.print(ax);Serial.print(",");Serial.print(ay);Serial.print(",");Serial.print(az);Serial.print(",");
        myFile.print(ax);myFile.print(",");myFile.print(ay);myFile.print(",");myFile.print(az);myFile.print(",");
      
        digitalWrite(AD0[i],LOW);   // Para cambiar la dirección a 0x68 y dejar de leer esta IMU
      }
  
      // Para dejar de ocupar la dirección 0x68 (por seguridad) y que la utilice el RTC:
      digitalWrite(AD0[0],HIGH);digitalWrite(AD0[1],HIGH);digitalWrite(AD0[2],HIGH);digitalWrite(AD0[3],HIGH);digitalWrite(AD0[4],HIGH);  // Se ponen en dirección 0x69 a los MPU
      
      // Manda día de la semana:
      Serial.print(rtc.getDOWStr());
      Serial.print(" ");
      myFile.print(rtc.getDOWStr());
      myFile.print(" ");
  
      // Manda fecha:
      Serial.print(rtc.getDateStr());
      Serial.print(" -- ");
      myFile.print(rtc.getDateStr());
      myFile.print(" -- ");
  
      // Manda tiempo:
      Serial.println(rtc.getTimeStr());
      myFile.println(rtc.getTimeStr());
  
      // Los regresa a la dirección 0x69 para poder volver a leerlos uno por uno:
      digitalWrite(AD0[0],LOW);digitalWrite(AD0[1],LOW);digitalWrite(AD0[2],LOW);digitalWrite(AD0[3],LOW);digitalWrite(AD0[4],LOW); // Se ponen en dirección 0x68 a los MPU

      myFile.close();
    }
  }
  digitalWrite(LED,LOW);    // Indicador de que dejó de registrar datos
  while(true){}    // Se queda ahí hasta que se reinicie el Arduino o se desenergice el sistema
}
