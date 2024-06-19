// Programa de MASTER Arduino MEGA Robot Limpiador de Playa - Adrián Ricárdez y Alejandro Ayala
// Máquina de estados con control Proporcional.
// La numeración de todos los sensores y actuadores se definieron desde la perspectiva del robot y de izquierda a derecha

#include <Servo.h>
// Declaración de Motores 99:1 Pololu
Servo Motor1;   // Motor superior izquierdo
Servo Motor2;   // Motor superior derecho
Servo Motor3;   // Motor inferior izquierdo
Servo Motor4;   // Motor inferior derecho
Servo Motor5;   // Rodillo  (Recordar que el controlador que tiene no sirve en un sentido, pero en el otro sí)
Servo Contenedor; // +//Controla el servo del contenedor de latas
Servo Camara1;    // Servo base cámara
Servo Camara2;    // Servo direccion cámara

// Variables Globales
const int u = 8;        // Número de ultrasónicos recibidos del Arduino UNO
const int LED1 = 32;
const int LED2 = 34;
const int LED3 = 36;
const int LED4 = 38;
const int buzzer = 53;
const int boton = 48;
int uS_1,uS_2,uS_3,uS_4,uS_5,uS_6,uS_7,uS_8;
int color, coordx, coordy;
int vi, vd;
int kp = 100;                     // Constante de proporción
int vf = 1800;                    // Velocidad constante forward
int vb = 1000;                    // Velocidad constante backward
int vr = 1800;                    // Velocidad constante del rodillo
int vp = 1500;                    // Velocidd de paro
int eu = 0;                       // Error de los ultrasónicos (-1 -2 -3 3 2 1)
int ec_x = 0;                     // Error de la cámara en x (0 1 2 3 4 5 6 7) manda la Rasp
int ec_y = 0;                     // Error de la cámara en y (0 1 2 3 4 5 6) manda la Rasp
int angulo_descarga = 97;         // Ángulo de descarga del servomotor del contenedor
int angulo_origen = 10;           // Ángulo de inicio del servomotor del contenedor
int ang_cam_srv1 = 15;            // Ángulo del servo de dirección de la camara
int ang_cam_srv2 = 9;            // Ángulo del servo base de la camara
int cont_boton = 0;               // Botón contador de uso general
boolean deposito_realizado = false; // Bandera de que ya se hizo por lo menos una vez el depósito
boolean flip = false;             // flip == false va a la izquierda

// ====================================== VOID SETUP =================================================================
void setup() {
  //Serial.begin(115200);     // Puerto serial para la comunicación con la computadora
  Serial2.begin(115200);    // Puerto serial para la recepción de datos de los ultrasónicos del SLAVE Arduino UNO
  Serial3.begin(115200);    // Puerto serial para la comunicación con la Raspberry Pi3
  // Inicialización de PWM para motores
  Motor1.attach(3);
  Motor2.attach(4);
  Motor3.attach(5);
  Motor4.attach(6);
  Motor5.attach(7);

  // Inicialización de PWM para la cámara y acomodo
  Camara1.attach(8);
  Camara2.attach(12);
  Camara1.write(ang_cam_srv1);
  Camara2.write(ang_cam_srv2);

  // Acomoda el contenedor en el ángulo de origen
  Contenedor.attach(11);
  Contenedor.write(angulo_origen);
  delay(1000);
  Contenedor.detach();  // Lo desactiva para que no siga consumiendo corriente
  
  // Declaración de pines de entrada y salida
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(boton, INPUT);
}

// ======================================= DATOS ULTRASÓNICOS Y PUERTO SERIAL ARDUINO UNO ================================================================
int leerUltrasonicos(int caso)
{
  if(Serial2.available()){
    String ultrasonicos = Serial2.readStringUntil(36);     // 36 es el ASCII de $, el fin de la cadena s(NUMERO0)a(NUMERO1)b(NUMERO2)c(NUMERO3)d(NUMERO4)e(NUMERO5)f(NUMERO)6g(NUMERO7)h$
    
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    int indice_0 = ultrasonicos.lastIndexOf('s');
    int indice_1 = ultrasonicos.indexOf('a',indice_0+1); //("a",0)
    int indice_2 = ultrasonicos.indexOf('b',indice_1+1); //("b",indice_1+1)
    int indice_3 = ultrasonicos.indexOf('c',indice_2+1); //("c",indice_2+1)
    int indice_4 = ultrasonicos.indexOf('d',indice_3+1); //("d",indice_3+1)
    int indice_5 = ultrasonicos.indexOf('e',indice_4+1); //("e",indice_4+1)
    int indice_6 = ultrasonicos.indexOf('f',indice_5+1); //("e",indice_5+1)
    int indice_7 = ultrasonicos.indexOf('g',indice_6+1); //("g",indice_6+1)
    int indice_8 = ultrasonicos.indexOf('h',indice_7+1); //("h",indice_7+1)
    
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = ultrasonicos.substring(indice_0+1,indice_1);
    String cadena_2 = ultrasonicos.substring(indice_1+1,indice_2);
    String cadena_3 = ultrasonicos.substring(indice_2+1,indice_3);
    String cadena_4 = ultrasonicos.substring(indice_3+1,indice_4);
    String cadena_5 = ultrasonicos.substring(indice_4+1,indice_5);
    String cadena_6 = ultrasonicos.substring(indice_5+1,indice_6);
    String cadena_7 = ultrasonicos.substring(indice_6+1,indice_7);
    String cadena_8 = ultrasonicos.substring(indice_7+1,indice_8);
    
    // Parse.Int de la cadena
    int uS_1 = cadena_1.toInt();
    int uS_2 = cadena_2.toInt();
    int uS_3 = cadena_3.toInt();
    int uS_4 = cadena_4.toInt();
    int uS_5 = cadena_5.toInt();
    int uS_6 = cadena_6.toInt();
    int uS_7 = cadena_7.toInt();
    int uS_8 = cadena_8.toInt();
    // Despliegue de vflores de los ultrasónicos
    /*Serial.print("Ultrasonico 1:");
    Serial.print(uS_1);
    Serial.print("  Ultrasonico 2:");
    Serial.print(uS_2);
    Serial.print("  Ultrasonico 3:");
    Serial.print(uS_3);
    Serial.print("  Ultrasonico 4:");
    Serial.print(uS_4);
    Serial.print("  Ultrasonico 5:");
    Serial.print(uS_5);
    Serial.print("  Ultrasonico 6:");
    Serial.print(uS_6);
    Serial.print("  Ultrasonico 7:");
    Serial.print(uS_7);
    Serial.print("  Ultrasonico 8:");
    Serial.println(uS_8);*/
    switch(caso)
    {
      case 1:
        return uS_1;
        break;
      case 2:
        return uS_2;
      break;
      case 3:
        return uS_3;
        break;
      case 4:
        return uS_4;
        break;
      case 5:
        return uS_5;
        break;
      case 6:
        return uS_6;
        break;
      case 7:
        return uS_7;
        break;
      case 8:
        return uS_8;
        break;
      default:
        break;
    }
  }
}

// ======================================= DATOS RASPBERRY PI DE LA CÁMARA ================================================================
int leerCamara(int caso)
{
  if(Serial3.available()!= 48){   // 48 es el ASCII del número 0
    String camara = Serial3.readStringUntil(122);     // 122 es el ASCII de z, el fin de la cadena c(1,2 o 3)x(Coordx)y(Coordy)hz
    
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    int index_0 = camara.lastIndexOf('c');
    int index_1 = camara.indexOf('x',index_0+1);
    int index_2 = camara.indexOf('y',index_1+1);
    int index_3 = camara.indexOf('h',index_2+1);
    int index_4 = camara.indexOf('z',index_3+1);
    
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String chain_1 = camara.substring(index_0+1,index_1);
    String chain_2 = camara.substring(index_1+1,index_2);
    String chain_3 = camara.substring(index_2+1,index_3);
    String chain_4 = camara.substring(index_3+1,index_4);
    
    // Parse.Int de la cadena
    int color = chain_1.toInt();
    int coordx = chain_2.toInt();
    int coordy =  chain_3.toInt();
    
    // Despliegue de valores (1 = Azul, 2 = Rojo, 3 = Negro)
    /*Serial.print("Color: ");
    Serial.print(color);
    Serial.print("  Coordenada x: ");
    Serial.print(coordx);
    Serial.print("  Coordenada y: ");
    Serial.println(coordy);*/
    
    switch(caso)
    {
      case 1:
        return color;
        break;
      case 2:
        return coordx;
        break;
      case 3:
        return coordy;
        break;
      default:
        break;
    }
  }
}

// ============================================ FUNCIONES  ==========================================================
void adelante()
{
  digitalWrite(buzzer,LOW);

    Motor1.writeMicroseconds(vf);
    Motor2.writeMicroseconds(vf);
    Motor3.writeMicroseconds(vf);
    Motor4.writeMicroseconds(vf);
    Motor5.writeMicroseconds(vr);   // Activar el rodillo
}

void estacionando_izq()
{
    Motor1.writeMicroseconds(vb+200);
    Motor2.writeMicroseconds(vb+500);
    Motor3.writeMicroseconds(vb+200);
    Motor4.writeMicroseconds(vb+50);
    delay(200);
}

void estacionando_der()
{
    Motor1.writeMicroseconds(vb+50);
    Motor2.writeMicroseconds(vb+200);
    Motor3.writeMicroseconds(vb+50);
    Motor4.writeMicroseconds(vb+200);
    delay(200);
}

void atras(int espera)
{
  digitalWrite(buzzer,LOW);
  Motor1.writeMicroseconds(vb);
  Motor2.writeMicroseconds(vb);
  Motor3.writeMicroseconds(vb);
  Motor4.writeMicroseconds(vb);
  Motor5.writeMicroseconds(vp);
  delay(espera);
}

void girar(int espera, int flippy)
{
  digitalWrite(buzzer,LOW);
  if(flippy == 1)   // giro motores izquierdos
  {
    Motor1.writeMicroseconds(1900);
    Motor2.writeMicroseconds(1000);
    Motor3.writeMicroseconds(1900);
    Motor4.writeMicroseconds(1000);
  }
  else if(flippy == 0)   // giro motores derechos
  {
    Motor1.writeMicroseconds(1000);
    Motor2.writeMicroseconds(1900);
    Motor3.writeMicroseconds(1000);
    Motor4.writeMicroseconds(1900);
  }
  Motor5.writeMicroseconds(vr);
  delay(espera);
}

void LEDs(int led1, int led2, int led3, int led4)
{
  digitalWrite(LED1, led1);
  digitalWrite(LED2, led2);
  digitalWrite(LED3, led3);
  digitalWrite(LED4, led4);
}

void depositar_contenedor()
{
  digitalWrite(buzzer,LOW);
  Motor1.writeMicroseconds(vp);
  Motor2.writeMicroseconds(vp);
  Motor3.writeMicroseconds(vp);
  Motor4.writeMicroseconds(vp);
  Motor5.writeMicroseconds(vp);
  // Depósito lento para evitar que la lata se quede abajo del contenedor o se salga
  Contenedor.attach(11);
  for(int i=angulo_origen;i<=angulo_descarga;i++)
  {
    Contenedor.write(i);
    delay(20);
  }
  // Vibración de descarga para que se caigan las latas
  for(int i=0;i<40;i++)
  {
    Contenedor.write(angulo_descarga-10);
    delay(80);
    Contenedor.write(angulo_descarga);
    delay(80);
  }
  // Regreso depósito lento
  for(int i=angulo_descarga;i>=(angulo_origen*2);i--)
  {
    Contenedor.write(i);
    delay(20);
  }
  // Multidescarga rápida 
  for(int i=0;i<3;i++)
  {
    Contenedor.write(angulo_descarga);
    delay(750);
    Contenedor.write(angulo_origen*2);
    delay(750);
  }
  // Multidescarga phantom
  for(int i=0;i<4;i++)
  {
    Contenedor.write(angulo_descarga);
    delay(550);
    Contenedor.write(angulo_origen*2);
    delay(550);
  }
  Contenedor.write(angulo_origen);
  delay(500);
  Contenedor.detach();

  deposito_realizado = true;    // Ya no volverá a entrar a depositar hasta que se reinicie el programa
}

// ===================================================== VOID LOOP ============================================================
void loop(){
  int cont_boton = 0;
  boolean estado_anterior = false;
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ INICIO DEL ROBOT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // El for es para darle tiempo a la Raspberry para que comience a ejecutar el script
  for(int i=0;i < 5;i++)
  {
    // KITT de LEDs
    digitalWrite(buzzer, LOW);
    LEDs(1,0,0,0);delay(40);
    LEDs(0,1,0,0);delay(40);
    LEDs(0,0,1,0);delay(40);
    digitalWrite(buzzer, HIGH);
    LEDs(0,0,0,1);delay(40);
    LEDs(0,0,1,0);delay(40);
    LEDs(0,1,0,0);delay(40);
    if(estado_anterior == false && digitalRead(boton))
    {
      estado_anterior = true;
      cont_boton++;
    }
    else if(estado_anterior == true && !digitalRead(boton))
      estado_anterior = false;
  }
  LEDs(1,1,1,1);
  delay(300);
  digitalWrite(buzzer, LOW);
  LEDs(0,0,0,0);delay(300);
  // RUTINAS
  if(cont_boton == 0)
    goto adel;                // Para ir a la rutina principal si funciona la cámara
      
/* --------------------------- Estado 1, Adelante ---------------------- */
  adel:
    LEDs(0,0,0,1);
    adelante();    // Ejecuta la función del motor "adelante()"
    // Lee los ultrasónicos
    uS_1=leerUltrasonicos(1);
    uS_2=leerUltrasonicos(2);
    uS_3=leerUltrasonicos(3);
    uS_4=leerUltrasonicos(4);
    uS_5=leerUltrasonicos(5);
    uS_6=leerUltrasonicos(6);
    uS_7=leerUltrasonicos(7);
    uS_8=leerUltrasonicos(8);
    
    // PRIORIDAD DE EVASIÓN DE OBJETOS Y AGUA
    // Detección de algún ultrasónico
    if((uS_1 != 0) || (uS_2 != 0) || (uS_3 != 0) || (uS_4 != 0) || (uS_5 != 0) || (uS_6 != 0))
      goto evasion;
    else    // Leer la cámara retrasa todo el programa aproximadamente por 800 ms
      // Lee la cámara
      color = leerCamara(1);
    // Detección de agua
    //if(color == 1)
      //goto rescate_agua;
    // JERARQUÍA MENOR DE CONTENEDOR
    // Detección de contenedor
    if(color == 2 && deposito_realizado == false)
      goto alinear_back_contenedor;

    goto adel;

/*----------------------------- Estado 2, Evasión ---------------------------*/  
  evasion:
    LEDs(0,0,1,0);
    
    // Lee todos los ultrasónicos
    uS_1=leerUltrasonicos(1);
    uS_2=leerUltrasonicos(2);
    uS_3=leerUltrasonicos(3);
    uS_4=leerUltrasonicos(4);
    uS_5=leerUltrasonicos(5);
    uS_6=leerUltrasonicos(6);
    uS_7=leerUltrasonicos(7);
    uS_8=leerUltrasonicos(8);
   
    eu = uS_1 + uS_2 + uS_3 + uS_4 + uS_5 + uS_6;
    // eu va de -1 -2 -3 3 2 1
    if(eu > 0)
      flip = 0;
    else
      flip = 1;
        
    // Velocidad de los motores
    vi = vf - kp * eu;    // Motor izquierdo
    vd = vf + kp * eu;    // Motor derecho

    // Para que no se pasen de 2000 ni baje menos de 1000, ya que es el rango de vflores del PWM
    if(vi > 1900)
      vi = 1900;
    else if(vi < 1100)
      vi = 1100;
    if(vd > 1900)
      vd = 1900;
    else if(vd < 1100)
      vd = 1100;

    // Escribir la velocidad de los motores
    Motor1.writeMicroseconds(vi);
    Motor2.writeMicroseconds(vd);
    Motor3.writeMicroseconds(vi);
    Motor4.writeMicroseconds(vd);
    Motor5.writeMicroseconds(vr);

    // PRIORIDAD DE EVASIÓN DE OBJETOS CENTRALES
    // Detección de sensores frontales
    if(uS_3 != 0 && uS_4 != 0)
      goto atras_girar;

    // JERARQUÍA MENOR DE ADELANTE Y 
    // Sin detección de ultrasónicos
    if(uS_1 == 0 && uS_2 == 0 && uS_3 == 0 && uS_4 == 0 && uS_5 == 0 && uS_6 == 0 && uS_7 == 0) 
      goto adel;
         
    goto evasion;    // Si no detecta ningún ultrasónico, va a evasión para confirmar
    
/*----------------------- Estado 3, Girar hacia atrás -----------------------------*/      
  atras_girar:
    LEDs(0,0,1,1);
    // Reacomodo del servomotor
    atras(1500);            // Se va a la función "atras()" con delay
    girar(1200,flip);       // Se va a la función "girar()" con delay
    if(uS_3 == 0 && uS_4 == 0)
      goto adel;
    goto evasion;

/*----------------------------- Estado 4, Rescate al detectar agua ---------------------------*/  
  rescate_agua:
    LEDs(0,1,0,0);
    // Lee la cámara
    coordx = leerCamara(2);
    if(coordx < 3)
      flip = 1;
    else
      flip = 0;

    girar(5,flip);
    color = leerCamara(1);
    if(color == 0)
      goto adel;
    else if(color == 1)
      goto rescate_agua;
/*--------------------------- Estado 6 y 7, Encontrar contenedor por ultrasónicos y retroceder ---------------------------------*/ 
  alinear_back_contenedor:
    LEDs(0,1,0,1);
    
    girar(2,flip);
    uS_7=leerUltrasonicos(7);
    uS_8=leerUltrasonicos(8);
    // Si los uS detectan por separado
    if(uS_7 != 0 && uS_8 == 0)
      estacionando_izq();
    if(uS_7 == 0 && uS_8 != 0)
      estacionando_der();
    // Si ambos ultrasonicos estan alineados
    if(uS_7 != 0 && uS_8 != 0)
    {
      LEDs(0,1,1,0);
      atras(1500);
      goto depositar;
    }
    goto alinear_back_contenedor;    // Girar infinitamente hasta que ambos ultrasonicos detecten alineamiento

/*--------------------------- Estado 8, Depositar al contenedor ---------------------------------*/ 
  depositar:
    LEDs(1,1,1,1);
    depositar_contenedor();
    goto adel;
}
