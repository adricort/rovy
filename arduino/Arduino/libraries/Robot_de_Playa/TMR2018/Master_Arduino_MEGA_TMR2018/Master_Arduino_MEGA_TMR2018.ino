
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

#include <Servo.h>
Servo Motor1;   // Motor superior izquierdo
Servo Motor2;   // Motor superior derecho
Servo Motor3;   // Motor inferior izquierdo
Servo Motor4;   // Motor inferior derecho
Servo Motor5;   // Rodillo  (Recordar que el controlador que tiene no sirve en un sentido, pero en el otro sí)
int vizq_adel = 1500;
int vizq_atr = 1500;
int vder_adel = 1500;
int vder_atr = 1500;
int vrodillo = 1500;

const int u = 8;        // Número de ultrasónicos recibidos del Arduino UNO
int uS_1,uS_2,uS_3,uS_4,uS_5,uS_6,uS_7,uS_8,uS_9;
int eu = 0;                       // Error de los ultrasónicos (-1 -2 -3 3 2 1)
int eu_atras = 0;
int v_crucero = 1700;
boolean flip = false;
int kp = 100;

// ====================================== VOID SETUP =================================================================
void setup() {
  Serial.begin(115200);     // Puerto serial para la comunicación con la computadora
  Serial1.begin(115200);    // Puerto serial para la recepción de datos de los ultrasónicos del SLAVE Arduino UNO
  Motor1.attach(3);
  Motor2.attach(5);
  Motor3.attach(4);
  Motor4.attach(6);
  Motor5.attach(7);
  
}
// ============================================ FUNCIONES  ==========================================================
// ======================================= DATOS ULTRASÓNICOS Y PUERTO SERIAL ARDUINO UNO ================================================================
int leerUltrasonicos(int caso)
{
  if(Serial1.available()){
    String ultrasonicos = Serial1.readStringUntil(36);     // 36 es el ASCII de $, el fin de la cadena s(NUMERO0)a(NUMERO1)b(NUMERO2)c(NUMERO3)d(NUMERO4)e(NUMERO5)f(NUMERO)6g(NUMERO7)h$    
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
    int indice_9 = ultrasonicos.indexOf('i',indice_8+1); //("i",indice_8+1)    
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = ultrasonicos.substring(indice_0+1,indice_1);
    String cadena_2 = ultrasonicos.substring(indice_1+1,indice_2);
    String cadena_3 = ultrasonicos.substring(indice_2+1,indice_3);
    String cadena_4 = ultrasonicos.substring(indice_3+1,indice_4);
    String cadena_5 = ultrasonicos.substring(indice_4+1,indice_5);
    String cadena_6 = ultrasonicos.substring(indice_5+1,indice_6);
    String cadena_7 = ultrasonicos.substring(indice_6+1,indice_7);
    String cadena_8 = ultrasonicos.substring(indice_7+1,indice_8);
    String cadena_9 = ultrasonicos.substring(indice_8+1,indice_9);
    // Parse.Int de la cadena
    int uS_1 = cadena_1.toInt();
    int uS_2 = cadena_2.toInt();
    int uS_3 = cadena_3.toInt();
    int uS_4 = cadena_4.toInt();
    int uS_5 = cadena_5.toInt();
    int uS_6 = cadena_6.toInt();
    int uS_7 = cadena_7.toInt();
    int uS_8 = cadena_8.toInt();
    int uS_9 = cadena_9.toInt();
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
    Serial.print(uS_8);
    Serial.print("  Ultrasonico 9:");
    Serial.print(uS_9);
    */
    switch(caso)
    {
      case 1:
        eu = uS_1 + uS_2 + uS_3 + uS_5 + uS_6 + uS_7;
        return eu;
        break;
      case 2:
        return uS_4;
        break;
      case 3:
        eu_atras = uS_8 - uS_9;
        return eu_atras;
        break;
      default:
        break;
    }
  }
}

// ===================================================== VOID LOOP ============================================================
void loop(){      
/* --------------------------- Estado 1, Adelante ---------------------- */
  ADELANTE:{
    Serial.print("  Estado:");
    Serial.print("ADELANTE");
    Motor1.writeMicroseconds(v_crucero);
    Motor2.writeMicroseconds(v_crucero);
    Motor3.writeMicroseconds(v_crucero);
    Motor4.writeMicroseconds(v_crucero);
    Motor5.writeMicroseconds(vrodillo);
    Serial.print("  Velocidad:");
    Serial.println(v_crucero);
    // Lee Ultrasonicos    
    if(leerUltrasonicos(1) != 0 || leerUltrasonicos(2) != 0)
      goto EVASION;   
    goto ADELANTE;
  }
  
  EVASION:{
    Serial.print("  Estado:");
    Serial.print("EVASION");
    // Lee los ultrasónicos
    int EU = leerUltrasonicos(1);
    Serial.print("  error:");
    Serial.print(EU);
    // eu va de -1 -2 -3 3 2 1
    if(EU > 0)
      flip = 1;
    else
      flip = 0;
      
    // Velocidad de los motores
    vizq_adel = v_crucero + kp * EU;    // Motor izquierdo
    constrain(vizq_adel,500,2000);
    vizq_atr = vizq_adel;//vf - kp * eu;    // Motor izquierdo
    vder_adel = v_crucero - kp * EU;    // Motor derecho
    constrain(vder_adel,500,2000);
    vder_atr = vder_adel; //vf + kp * eu;    // Motor derecho
    
    Motor1.writeMicroseconds(vizq_adel);
    Motor2.writeMicroseconds(vizq_atr);
    Motor3.writeMicroseconds(vder_adel);
    Motor4.writeMicroseconds(vder_atr);
    Motor5.writeMicroseconds(vrodillo);
    Serial.print("  izquierda:");
    Serial.print(vizq_adel);
    Serial.print("  derecha:");
    Serial.println(vder_adel);
    if(leerUltrasonicos(1) == 0 && leerUltrasonicos(2) == 0)
      goto ADELANTE;
         
    goto EVASION;
  }
    
}
