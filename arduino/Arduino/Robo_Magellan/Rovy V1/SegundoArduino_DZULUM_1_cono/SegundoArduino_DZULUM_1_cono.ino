/* CLUB DE ROBÓTICA DE LA FACULTAD DE INGENIERÍA- Equipo: SPARTAN CROFI
 PROGRAMA DEL SEGUNDO ARDUINO DZULÚM (ROVY).
CATEGORÍA: ROBO MAGELLAN
 SECCIÓN DE POTENCIA
Valores de PWM para los motores:
Adelante: 197-253 Donde el 197 es el más lento y el 253 es el más rápido
Desarrollado por: Ricárdez Ortigosa Adrián
Atrás: 1-185 Donde el 185 es el más lento y el 1 es el más rápido
Paro: 186-196 El promedio de PARO es 191
Materiales necesarios:
- 1 brújula GY-271
- 2 Arduinos UNO
Conexiones además de VCC y GND:
Segundo Arduino Brújula
A4 --> SDA
A5 --> SCL
Segundo Arduino Primer Arduino
pin 7 --> pin 6
pin 6 --> pin 7
Primer Arduino Segundo Arduino
pin 8 --> pin 2 ("0" Interrupción)
*/
// =========== BIBLIOTECAS ================
#include "I2Cdev.h" // Biblioteca para MPU6050
#include "MPU6050_6Axis_MotionApps20.h" // Biblioteca para MPU6050
#include "MPU6050.h" // Biblioteca para MPU6050
#include <SoftwareSerial.h> // Biblioteca necesaria para multiplexear los puertos seriales del Arduino
// ======== MULTIPLEXEO DE PUERTO SERIAL ===========
SoftwareSerial Arduinos(6,7); // Arduinos(Rx,Tx)
// ======= VARIABLES GLOBALES =============
float thetha_grados; // Ángulo que va a recibir vía Serial del PRIMER ARDUINO
float alfa_grados; // Ángulo actual del robot (del MPU6050)
float angulo_z; // Ángulo del eje z del MPU6050
float offset_Norte; // Ángulo en forma de offset para marcar el Norte del Robot (inicio)
float PWM_Izq=191, PWM_Der=191; // Variables para los motores de Rovy, empiezan en "PARO"
const int Mot_Izq = 9, Mot_Der = 10; // Pines digitales del Arduino para los PWM de los Motores
const int interruptor = 8; // Pin que leerá del otro Arduino si ya llegó a su destino
const int LED_calibrado = 12; // LED que indicará si ya se calibró el MPU6050
const int camara_check = 11; // Pin que enviará el pulso de terminación de la cámara
boolean exito = false; // Variable booleana que determina si llegó o no a su destino
boolean inicio = false; // Variable booleana que indica que ya se está calibrando el MPU6050
boolean calibrado = false; // Variable booleana que indica que ya se calibró el MPU6050
boolean calculo_offset = false; // Variable booleana que indica si ya se hizo el cálculo deloffset del ángulo 
// ============== VARIABLES CÁMARA =================
int PWM_cte = 224;
int Cte_4 = 10;
int Decremento_1;
int Decremento_2;
int N_PWMA;
int N_PWMB;
// ============== CÁMARA STUFF =================
 char cadena[30]; //Creamos un array que almacenará los caracteres que escribiremos en la consola del PC. Le asignamos un tope de caracteres, en este caso 30
 byte posicion=0; //Variable para cambiar la posición de los caracteres del array
 int valor; //Variable del valor entero
// ============== MPU6050 STUFF =================
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
 #include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL
#define LED_PIN 13
bool blinkState = false;
bool dmpReady = false; // set true if DMP init was successful
uint8_t mpuIntStatus; // holds actual interrupt status byte from MPU
uint8_t devStatus; // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize; // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount; // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q; // [w, x, y, z] quaternion container
VectorInt16 aa; // [x, y, z] accel sensor measurements
VectorInt16 aaReal; // [x, y, z] gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z] world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z] gravity vector
float euler[3]; // [psi, theta, phi] Euler angle container
float ypr[3]; // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
 mpuInterrupt = true;}
// ============================================
// ============== VOID SETUP ==================
// ============================================
void setup(){

 // ============== MPU6050 STUFF =================
 // join I2C bus (I2Cdev library doesn't do this automatically)
 #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
 Wire.begin();
 TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
 #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
 Fastwire::setup(400, true);
 #endif
 Serial.begin(9600); // Serial Monitor
 while (!Serial); // wait for Leonardo enumeration, others continue immediately
 mpu.initialize();
 devStatus = mpu.dmpInitialize();
 // supply your own gyro offsets here, scaled for min sensitivity
 mpu.setXGyroOffset(220);
 mpu.setYGyroOffset(76);
 mpu.setZGyroOffset(-85);
 mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
 // make sure it worked (returns 0 if so)
 if (devStatus == 0) {
 // turn on the DMP, now that it's ready
 //Serial.println(F("Enabling DMP..."));
 mpu.setDMPEnabled(true);
 attachInterrupt(0, dmpDataReady, RISING);
 mpuIntStatus = mpu.getIntStatus();
 dmpReady = true;
 packetSize = mpu.dmpGetFIFOPacketSize();
 }
 pinMode(LED_PIN, OUTPUT);
 // ============== FIN MPU6050 STUFF =================
 Arduinos.begin(9600); // Serial Arduinos
 pinMode(interruptor,INPUT);
 pinMode(LED_calibrado,OUTPUT);
 pinMode(Mot_Izq, OUTPUT);
 pinMode(Mot_Der, OUTPUT);
 pinMode(camara_check,OUTPUT);
 digitalWrite(camara_check,LOW);
}
// ===========================================
// ============== VOID LOOP ==================
// ===========================================
void loop() {
 
 // ============== CALIBRACIÓN MPU6050 ===============
 if( calibrado == false ){
 for(int k=0;k<10;k++){ // CAMBIAR A 2000
 if(inicio == false){
 //Serial.println("CALIBRANDO MPU6050, ESPERE UN MOMENTO POR FAVOR...");
 //Serial.println("");
 inicio = true;
 }
 Serial.println(k);
 MPU_6050(); // Función de lecturas de ángulos
 }
 calibrado = true; // Ya está calibrado el MPU6050
 if( calculo_offset == false ){
 offset_Norte = 90 - angulo_z; // offset_Norte tomará el valor del PRIMER ángulo para marcar el Norte, una vez
 calculo_offset = true; // Ya se hizo el cálculo, ya no se volverá a hacer
 digitalWrite(LED_calibrado,HIGH);
 }
 }

 //Serial.println("Esperando a recibir el angulo de direccion del PRIMER ARDUINO...");
 while(Arduinos.available()==0){} // Espera la respuesta del dato que el primer Arduino va a dar por el puerto serial Arduinos(Rx,Tx)
 thetha_grados= Arduinos.parseFloat(); // Lee únicamente los números flotantes que recibe del primer Arduino del Arduinos.print(theta_grados)
 // Conversión de ángulos de "0° a 360°" a "0 a 180° y -180° a 0°"
 if( thetha_grados > 180 ){
 thetha_grados= thetha_grados-360;}
 /*
 Serial.print("El angulo de direccion en grados es de: ");
 Serial.print(thetha_grados);Serial.println(" grados"); // Del Ejemplo: 64.5379°
 Serial.print("Angulo del robot,");
 Serial.print("PWM_Izq,");
 Serial.println("PWM_Der");*/

 while(true){
 MPU_6050();
 angulo_z= angulo_z + offset_Norte;
 if( angulo_z >= 0){
 alfa_grados= angulo_z*(-1)+180;} // Transforma los datos recibidos del gyro en 0 a 180° y 0 a -180°
 else{ // offset_Norte es generalmente 46°
 alfa_grados= (angulo_z+180)*(-1);}

 PWM_Izq= 220+(alfa_grados-thetha_grados)*0.717; // 0.717 fue el factor más grande paraque avance 253 PWM a 60° de diferencia entre ángulos
 PWM_Der= 220+(thetha_grados-alfa_grados)*0.717; // 0.717 fue el factor más grande paraque avance 253 PWM a 60° de diferencia entre ángulos
 // Límites de avance, porque si no, comienza a hacer estupideces el robot XD
 if( PWM_Izq >= 253 )
 PWM_Izq = 253;
 if( PWM_Der >= 253 )
 PWM_Der = 253;

 // POTENCIA
 analogWrite(Mot_Izq,PWM_Izq);
 analogWrite(Mot_Der,PWM_Der);
 //Serial.print(",");
 //Serial.print(PWM_Izq);Serial.print(",");
 //Serial.println(PWM_Der);
  if( digitalRead(interruptor) == HIGH ){   // SI YA LLEGÓ AL 80% DE ERROR RELATIVO DEL OTRO ARDUINO
    analogWrite(Mot_Izq,191);
    analogWrite(Mot_Der,191);
    while(true){
    if(Serial.available()){ //Nos dice si hay datos dentro del buffer
 memset(cadena, 0,sizeof(cadena));//memset borra el contenido del array "cadena" desde laposición 0 hasta el final sizeof
 while(Serial.available()>0) //Mientras haya datos en el buffer ejecuta la función
 {
 delay(5); //Poner un pequeño delay para mejorar la recepción de datos
 cadena[posicion]=Serial.read();//Lee un carácter del string "cadena" de la "posicion", luego leeel siguiente carácter con "posicion++"
 posicion++;
 }
 valor=atoi(cadena);//Convertimos la cadena de caracteres en enteros
 //Serial.println(valor);//Imprimimos el valor sumandole un valor +2
 posicion=0;//Ponemos la posicion a 0

 if((valor > 100) && (valor <= 300))    // VIO EL CONO HACIA LA IZQUIERDA
 {

 Decremento_1 = valor / Cte_4 ;
 analogWrite(Mot_Der, PWM_cte);
 analogWrite(Mot_Izq, N_PWMB);
 N_PWMB = PWM_cte - Decremento_1;

 Serial.println(N_PWMB);

 //Serial.println(valor);
 }
 else if((valor >= 0) && (valor <= 30)) {     // VIO EL CONO HACIA LA DERECHA

 Decremento_2 = valor;
 N_PWMA = PWM_cte - Decremento_2;
 analogWrite(Mot_Der, N_PWMA);
 analogWrite(Mot_Izq, PWM_cte);

 Serial.println(N_PWMA);
 }
 else if(valor == 1000){    // YA TOCÓ EL CONO
 analogWrite(Mot_Der, 191);
 analogWrite(Mot_Izq, 191);
 Serial.println(valor);
 delay(1000);

 analogWrite(Mot_Der, 162);
 analogWrite(Mot_Izq, 220);
 Serial.println(valor);
 digitalWrite(camara_check,HIGH);
 delay(3000);
 analogWrite(Mot_Der, 191);
 analogWrite(Mot_Izq, 191);
 digitalWrite(camara_check,LOW);
 break;
 }
 else {                           // BÚSQUEDA DEL CONO
 analogWrite(Mot_Der,162);
 analogWrite(Mot_Izq, 220);
 Serial.println(valor);
 }// END ELSE
}// END IF SERIAL AVAILABLE 
}// END WHILE TRUE
//break;
 }// END IF (PULSO DE INTERRUPCIÓN)
 }// END WHILE TRUE UNA VEZ QUE YA RECIBIÓ EL ÁNGULO DEL OTRO ARDUINO VIA SERIAL
}// END LOOP
// ==================================================
// =================== FUNCIONES ====================
// ==================================================
void MPU_6050(){
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
 Serial.println(F("FIFO overflow!"));
 // otherwise, check for DMP data ready interrupt (this should happen frequently)
 }
 else if (mpuIntStatus & 0x02) {
 // wait for correct available data length, should be a VERY short wait
 while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
 // read a packet from FIFO
 mpu.getFIFOBytes(fifoBuffer, packetSize);
 // track FIFO count here in case there is > 1 packet available
 // (this lets us immediately read more without waiting for an interrupt)
 fifoCount -= packetSize;

 // ============= DESPLIEGUE DE VALORES ===============
 // ANGULOS EN GRADOS [°]
 #ifdef OUTPUT_READABLE_YAWPITCHROLL
 // display Euler angles in degrees
 mpu.dmpGetQuaternion(&q, fifoBuffer);
 mpu.dmpGetGravity(&gravity, &q);
 mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
 angulo_z= ypr[0]*180/M_PI;
 #endif
 }
 blinkState = !blinkState;
 digitalWrite(LED_PIN, blinkState);
}
