// PROGRM MPU6050: Adrián Ricárdez Ortigosa

// Variables
int gx, gy, gz;
// Para ángulos en 360°
float angulo_x;
float angulo_y;
float angulo_z;
const int u = 5;                                    // Número de sensores
const int TriggerPin[u] = {4,6,8,10,13};    
const int EchoPin[u] = {5,7,9,11,12};      
unsigned long Duration[u] = {0,0,0,0,0};      // La duración del Echo por el Trigger
long Distancia_mm[u] = {0,0,0,0,0};           // La distancia devuelta de la función fDistancia()
int rango_us = 8000;                          // Rango recomendado para un metro de distancia: 6000
int valor_digitalizacion = 900;               // Si detecta abajo de ese valor, seran 1, sino 0

// ======================================================================

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "MPU6050.h"

// Arduino Wire library   is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
MPU6050 mpu(0x68);

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO.
#define OUTPUT_READABLE_YAWPITCHROLL

#define LED_PIN 13
bool blinkState = false;

// ================================ VARIABLES GLOBALES ==================================================
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

// ===================================== INTERRUPCIÓN =========================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

// ======================================= VOID SETUP ===========================================

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);
    
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    mpu.initialize();   // Inicialización del MPU

    // load and configure the DMP
    //Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // Offsets
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 default de fábrica

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        //Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
    pinMode(LED_PIN, OUTPUT);

    // Para los ULTRASÓNICOS
    for(int i=0;i<u;i++)
    {
      pinMode(TriggerPin[i],OUTPUT);
      pinMode(EchoPin[i],INPUT);
    }
}

// ====================================== VOID LOOP =============================================

void loop() {
      // if programming failed, don't try to do anything
      if (!dmpReady) return;
  
      // wait for MPU interrupt or extra packet(s) available
      while (!mpuInterrupt && fifoCount < packetSize){}
  
      // reset interrupt flag and get INT_STATUS byte
      mpuInterrupt = false;
      mpuIntStatus = mpu.getIntStatus();
  
      // get current FIFO count
      fifoCount = mpu.getFIFOCount();
  
      // check for overflow (this should never happen unless our code is too inefficient)
      if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
          // reset so we can continue cleanly
          mpu.resetFIFO();
          //Serial.println(F("FIFO overflow!"));
  
      // otherwise, check for DMP data ready interrupt (this should happen frequently)
      } else if (mpuIntStatus & 0x02) {
          // wait for correct available data length, should be a VERY short wait
          while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
  
          // read a packet from FIFO
          mpu.getFIFOBytes(fifoBuffer, packetSize);
          
          // track FIFO count here in case there is > 1 packet available
          // (this lets us immediately read more without waiting for an interrupt)
          fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            angulo_x= ypr[2] * 180/M_PI;        //  ypr[2] es en x  // M_PI= 3.14
            angulo_y= ypr[1] * 180/M_PI;        //  ypr[1] es en y
            angulo_z= ypr[0] * 180/M_PI;        //  ypr[0] es en z 
            
            if( angulo_x < 0 )
              angulo_x= 360+angulo_x;
            if( angulo_y < 0 )
              angulo_y= 360+angulo_y;
            if( angulo_z < 0 )
              angulo_z= 360+angulo_z;

            angulo_z = 360 - angulo_z;

            // Sensado de Ultrasónicos
            for(int i=0;i<u;i++)
            {
              // Obtención de datos de los sonares
              digitalWrite(TriggerPin[i],HIGH);
              delayMicroseconds(10);
              digitalWrite(TriggerPin[i],LOW);
              Duration[i] = pulseIn(EchoPin[i], HIGH, rango_us);
              
              Distancia_mm[i] = fDistancia(Duration[i]);

              if(Distancia_mm[i] > 100)
                Distancia_mm[i] = 1;
              else
                Distancia_mm[i] = 0;

              /*if(Distancia_mm[i] < valor_digitalizacion && Distancia_mm[i] != 0)
                Distancia_mm[i] = 1;
              else
                Distancia_mm[i] = 0;*/
            }
            // De menor a mayor la distancia, para que no mande números tan bruscos
            /*for(int i=0;i<u;i++)
            {
              // Mientras más lejos está el objeto, más pequeño es el número (para que la respuesta del Mega sea suave)
              if(Distancia_mm[i] != 0)
                Distancia_mm[i] = 1000 - Distancia_mm[i];   // 1000 porque debe empezar a detectar aproximadamente desde esa distancia
            }*/
            
            /*Serial.print("ANGULOS de 0 a 360:  ");
            Serial.print(",");   
            Serial.print(angulo_x,4);
            Serial.print(",");
            Serial.print(angulo_y,4);   
            Serial.println(",");*/
            
            Serial.print("s");Serial.print(Distancia_mm[0]);Serial.print("a");
            Serial.print(Distancia_mm[1]);Serial.print("b");
            Serial.print(Distancia_mm[2]);Serial.print("c");
            Serial.print(Distancia_mm[3]);Serial.print("d");
            Serial.print(Distancia_mm[4]);Serial.print("e");
            Serial.print(angulo_z,2);Serial.print("f");Serial.println("$");
                        
        #endif

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}

// ---------------------------- FUNCIONES -------------------------------------------------------------------
long fDistancia(long tiempo) {
  // Calculamos la distancia en mm
  // ((tiempo)*(Velocidad del sonido)/ el camino se hace dos veces)

  long DistanceCalc; // Variable para los cálculos
  DistanceCalc = (tiempo / 2.9) / 2; // Cálculos en milímetros
  // DistanceCalc = (tiempo /29) / 2; // Cálculos en centímetros
  // DistanceCalc = (tiempo / 74) / 2; // Cálculos en pulgadas
  return DistanceCalc; // Devolvemos el calculo
}
