// PROGRAMA SPHYRNIDAE POR: SIGMA- CURSO DE ROBOTS SEGUIDORES DE LINEA CROFI (CLUB DE ROBÓTICA DE LA FACULTAD DE INGENERÍA)
//----------------------------------------------------------------------------------------------------------------- VALORES, VARIABLES Y ARREGLOS
#include <Servo.h>
Servo myservo;
// SENSORES Y SEÑALES
const int sensor_[] = {A4,A3,A2,A1,A0};
int senal_[5];
// MOTORES Y VELOCIDADES
const int mot_der_adelante = 7;
const int mot_der_atras = 4;
const int mot_izq_adelante = 8;
const int mot_izq_atras = 10;
const int vel_mot_izq = 6;    // PWM
const int vel_mot_der = 5;   // PWM
// LEDS Y PUSH BUTTON
const int LED_senal_izq = 12;
const int LED_senal_der = 13;
const int push_inicio = 11;
const int push_cont = 2;
// CORRECCIONES E INDICADORES DE VUELTA
int correccion_izq = 0;
int correccion_der = 0;
int correccion_servo_izq = 0;
int correccion_servo_der = 0;
int contador1 = 0;
int contador2 = 0;
boolean sensor_invisible_izq = false;    // Sensores invisibles que nos ayudarán a indicar una dirección cuando éste se pierda
boolean sensor_invisible_der = false;
boolean ultimo_est_inicio = false;
boolean ultimo_est_cont = false;
// DEMÁS VARIABLES NECESARIAS PARA EL PROGRAMA
int i;
boolean inicio = false;

//----------------------------------------------------------------------------------------------------------------- VOID SETUP()  
void setup(){
  //Serial.begin(9600);
  for(i=0;i<5;i++)
    pinMode(sensor_[i], INPUT);
  pinMode(push_inicio, INPUT);
  pinMode(push_cont, INPUT);
  pinMode(LED_senal_izq, OUTPUT);
  pinMode(LED_senal_der, OUTPUT);
  pinMode(mot_izq_adelante, OUTPUT);
  pinMode(mot_izq_atras, OUTPUT);
  pinMode(mot_der_adelante, OUTPUT);
  pinMode(mot_der_atras, OUTPUT);
  pinMode(vel_mot_izq, OUTPUT);
  pinMode(vel_mot_der, OUTPUT);
  myservo.attach(3);  // Declaro al pin 2 digital como el controlador de mi Servomotor
  myservo.write(98);  // Valor del servo encontrado a 90° reales 
} 

//----------------------------------------------------------------------------------------------------------------- VOID LOOP()
void loop(){        
   /* PROGRAMA PARA HACER TABLA DE VELOCIDADES DE LOS MOTORES:
   int estado_boton_cont= digitalRead(push_inicio);
   if( estado_boton_cont == HIGH && ultimo_est_cont == LOW ){
    ultimo_est_cont = true;
    contador1 = contador1 + 2;
    digitalWrite(LED_senal_izq, HIGH);
    delay(200);
    digitalWrite(LED_senal_izq, LOW);
   }
   if( estado_boton_cont == LOW && ultimo_est_cont == HIGH )
    ultimo_est_cont = false;
   
   int estado_boton_inicio= digitalRead(push_cont);
   if( estado_boton_inicio == HIGH && ultimo_est_inicio == LOW ){
    ultimo_est_inicio = true;
    contador2 = contador2 + 2;
    digitalWrite(LED_senal_der, HIGH);
    delay(200);
    digitalWrite(LED_senal_der, LOW);
   }
   if( estado_boton_inicio == LOW && ultimo_est_inicio == HIGH )
    ultimo_est_inicio= false;
   
   digitalWrite(mot_izq_adelante, HIGH);
   digitalWrite(mot_der_adelante, HIGH);
   analogWrite(vel_mot_izq, contador1);		
   analogWrite(vel_mot_der, contador2);
   
   Serial.print("Velocidad motor izquierdo: ");
   Serial.print(contador2);
   Serial.print("  Velocidad motor derecho: ");
   Serial.println(contador1);*/
   
  // PROGRAMA PRINCIPAL
  //Botones de inicio y contador:
  digitalWrite(LED_senal_der,HIGH);
  int estado_boton = digitalRead(push_inicio);
  if( estado_boton == HIGH ){
    delay(1000);
    digitalWrite( LED_senal_izq, HIGH );digitalWrite( LED_senal_der, HIGH );delay(100);digitalWrite( LED_senal_izq, LOW );digitalWrite( LED_senal_der, LOW );delay(900);
    digitalWrite( LED_senal_izq, HIGH );digitalWrite( LED_senal_der, HIGH );delay(100);digitalWrite( LED_senal_izq, LOW );digitalWrite( LED_senal_der, LOW );delay(900);
    digitalWrite( LED_senal_izq, HIGH );digitalWrite( LED_senal_der, HIGH );delay(100);digitalWrite( LED_senal_izq, LOW );digitalWrite( LED_senal_der, LOW );delay(900);
    digitalWrite( LED_senal_izq, HIGH );digitalWrite( LED_senal_der, HIGH );delay(100);digitalWrite( LED_senal_izq, LOW );digitalWrite( LED_senal_der, LOW );delay(900);
    digitalWrite( LED_senal_izq, HIGH );digitalWrite( LED_senal_der, HIGH );
    inicio = true;
  }
  
  // INICIO:
  while( inicio == true ){
    for(i=0;i<5;i++)
      senal_[i] = digitalRead(sensor_[i]);
    
    // CORRECCIONES
    // Corrección centro:
    if( senal_[0] == HIGH && senal_[1] == HIGH && senal_[2] == LOW && senal_[3] == HIGH && senal_[4] == HIGH)
      centrado();
    if( senal_[0] == HIGH && senal_[1] == LOW && senal_[2] == LOW && senal_[3] == LOW && senal_[4] == HIGH)
      centrado();
    // Corrección izquierda:
    if( senal_[0] == HIGH && senal_[1] == LOW && senal_[2] == LOW && senal_[3] == HIGH && senal_[4] == HIGH)
      correccion_izquierda_1();
    if( senal_[0] == HIGH && senal_[1] == LOW && senal_[2] == HIGH && senal_[3] == HIGH && senal_[4] == HIGH)
      correccion_izquierda_2();
    if( senal_[0] == LOW && senal_[1] == LOW && senal_[2] == HIGH && senal_[3] == HIGH && senal_[4] == HIGH)
      correccion_izquierda_3();
    if( senal_[0] == LOW && senal_[1] == HIGH && senal_[2] == HIGH && senal_[3] == HIGH && senal_[4] == HIGH)
      correccion_izquierda_4();
    if( sensor_invisible_izq == HIGH && senal_[0] == HIGH && senal_[1] == HIGH && senal_[2] == HIGH && senal_[3] == HIGH && senal_[4] == HIGH)
      perdido_izquierda();
    // Corrección derecha:
    if( senal_[0] == HIGH && senal_[1] == HIGH && senal_[2] == LOW && senal_[3] == LOW && senal_[4] == HIGH)
      correccion_derecha_1();
    if( senal_[0] == HIGH && senal_[1] == HIGH && senal_[2] == HIGH && senal_[3] == LOW && senal_[4] == HIGH)
      correccion_derecha_2();
    if( senal_[0] == HIGH && senal_[1] == HIGH && senal_[2] == HIGH && senal_[3] == LOW && senal_[4] == LOW)
      correccion_derecha_3();
    if( senal_[0] == HIGH && senal_[1] == HIGH && senal_[2] == HIGH && senal_[3] == HIGH && senal_[4] == LOW)
      correccion_derecha_4();
    if( senal_[0] == HIGH && senal_[1] == HIGH && senal_[2] == HIGH && senal_[3] == HIGH && senal_[4] == HIGH && sensor_invisible_der == HIGH)
      perdido_derecha();
    
    // ESPECIFIACIONES MOTORES Y SERVOMOTOR
    /*digitalWrite(mot_izq_adelante, HIGH);
    digitalWrite(mot_der_adelante, HIGH);
    digitalWrite(mot_izq_atras, LOW);
    digitalWrite(mot_der_atras, LOW);*/
    
    analogWrite(vel_mot_izq, 100 - correccion_izq );
    analogWrite(vel_mot_der, 122 - correccion_der );
    digitalWrite(mot_der_adelante, HIGH);
    digitalWrite(mot_izq_adelante, HIGH);
    
    myservo.write(98 - correccion_servo_izq + correccion_servo_der);    // Valor del servo encontrado a 90° reales
    
    /*Serial.print(senal_[0]);
    Serial.print("  ");
    Serial.print(senal_[1]);
    Serial.print("  ");
    Serial.print(senal_[2]);
    Serial.print("  ");
    Serial.print(senal_[3]);
    Serial.print("  ");
    Serial.println(senal_[4]);
    */
  }
}

//--------------------------------------------------------------------------------------------------------------------- FUNCIONES
// Correcciones:
void centrado(){
  correccion_izq = 0;
  correccion_der = 0;
  sensor_invisible_izq = false;
  sensor_invisible_der = false;
  correccion_servo_izq = 0;
  correccion_servo_der = 0;
}
void correccion_izquierda_1(){
  correccion_izq = 6;
  correccion_der = 0;
  sensor_invisible_izq = true;
  sensor_invisible_der = false;
  correccion_servo_izq = 0;
  correccion_servo_der = 2;
}
void correccion_derecha_1(){
  correccion_izq = 0;
  correccion_der = 3;
  sensor_invisible_izq = false;
  sensor_invisible_der = true;
  correccion_servo_izq = 2;
  correccion_servo_der = 0;
}
void correccion_izquierda_2(){
  correccion_izq = 11;
  correccion_der = 0;
  sensor_invisible_izq = true;
  sensor_invisible_der = false;
  correccion_servo_izq = 6;
  correccion_servo_der = 0;
}
void correccion_derecha_2(){
  correccion_izq = 0;
  correccion_der = 20;
  sensor_invisible_izq = false;
  sensor_invisible_der = true;
  correccion_servo_izq = 0;
  correccion_servo_der = 6;
}
void correccion_izquierda_3(){
  correccion_izq = 30;
  correccion_der = 0;
  sensor_invisible_izq = true;
  sensor_invisible_der = false;
  correccion_servo_izq = 15;
  correccion_servo_der = 0;
}
void correccion_derecha_3(){
  correccion_izq = 0;
  correccion_der = 32;
  sensor_invisible_izq = false;
  sensor_invisible_der = true;
  correccion_servo_izq = 0;
  correccion_servo_der = 15;
}
void correccion_izquierda_4(){
  correccion_izq = 66;
  correccion_der = 20;
  sensor_invisible_izq = true;
  sensor_invisible_der = false;
  correccion_servo_izq = 25;
  correccion_servo_der = 0;
}
void correccion_derecha_4(){
  correccion_izq = 18;
  correccion_der = 62;
  sensor_invisible_izq = false;
  sensor_invisible_der = true;
  correccion_servo_izq = 0;
  correccion_servo_der = 25;
}
void perdido_izquierda(){
  correccion_izq = 100;
  correccion_der = 20;
  sensor_invisible_izq = true;
  sensor_invisible_der = false;
  correccion_servo_izq = 30;
  correccion_servo_der = 0;
}
void perdido_derecha(){
  correccion_izq = 25;
  correccion_der = 122;
  sensor_invisible_izq = false;
  sensor_invisible_der = true;
  correccion_servo_izq = 0;
  correccion_servo_der = 30;
}
