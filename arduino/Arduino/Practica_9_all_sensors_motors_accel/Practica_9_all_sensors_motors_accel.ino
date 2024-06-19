// Programa hecho por Adrián Ricárdez y Ricardo Osvaldo

#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

//Motor izquierdo 
#define MLA 3
#define MLB 5

//Motor derechos
#define MRA 6
#define MRB 9

//PWM's para manejo de motores derecho e izquierdo
int pwmLA;
int pwmLB;
int pwmRA;
int pwmRB;

//Funcion del subscriptor, transforma el pwm en positivo
void speedsCallback(const std_msgs::Float32MultiArray& msg){

  //Motor izquierdo
    if(msg.data[1]>=0){
       pwmLA = 0;
       pwmLB = (int)(255*msg.data[1]);
    }else{
       pwmLA = (int)(-255*msg.data[1]);
       pwmLB = 0;
    }
  //Motor derecho
    if(msg.data[0]>=0){
       pwmRA = 0;
       pwmRB = (int)(255*msg.data[0]);
    }else{
       pwmRA = (int)(-255*msg.data[0]);
       pwmRB = 0;
    }
}

//Crea un arreglo de tipo multif32 de la libreria std_msgs
std_msgs::Float32MultiArray data_console;

//Crea un nodo en ros llamado nh
ros::NodeHandle nodito;

//Crear publicador pub_sen en topico de minirobot
ros::Publisher publica("/minirobot/hardware/sensors", &data_console);

//Mi subscriptor subSpeeds se suscribe al topico de minirobot/hardwar/motor_speeds
ros::Subscriber<std_msgs::Float32MultiArray> subSpeeds("/minirobot/hardware/motor_speeds", &speedsCallback);

//Crea arreglo de sensores
float sens[15]={2,4,7,8,10,11,12,13,5,6,9,A0,A1,A3,A2};
float lect_sens[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float temperatura_anterior;

void setup(){ 

  //Inicializa el puerto a 500000 baudios, para buen funcionamiento de acelerometro con el filtro
  
  nodito.getHardware()->setBaud(500000);
  //Lee lecturas del acelerometro 
 
  
  //Iniciar nodito en ros
  nodito.initNode(); 

  //Advertir a nodito que voy a publicar
  nodito.advertise(publica);

  //Sucribir a nodito a subSpeeds
  nodito.subscribe(subSpeeds);

  //Declara tamaño del arreglo a publicar
  data_console.data_length = 15;
  
  //Declarar sensores de entrada
  for(int i=0; i<15; i++){
    pinMode(sens[i], INPUT);
  }

  //Declara pines de salida del arduino
    pinMode(MLA, OUTPUT);
    pinMode(MLB, OUTPUT);
    pinMode(MRA, OUTPUT);
    pinMode(MRB, OUTPUT);
}

void loop(){ 
  
 //Lee lectura de los sensores de presencia
  for(int j=0; j<8; j++){
    lect_sens[j] = digitalRead(sens[j]);   // Está en negativo ya que la placa tiene al revés la GND y Vcc en los sensores QRD1114 y se tuvieron que invertir mediante soldadura
  }

  //Lee sensore de temperatura, Bateria y dos foto resistencias
  for(int j=11; j<15; j++){
    lect_sens[j] = analogRead(sens[j]);
    if(j == 13)
      lect_sens[j] = analogRead(sens[j]) + 97;    // 97 es el offset
  }
  
  //Mueve motores al pwm que lo indica el mini_robot_gui
  analogWrite(MLA,pwmLA);
  analogWrite(MLB,pwmLB);
  analogWrite(MRA,pwmRA);
  analogWrite(MRB,pwmRB);

  //Paso datos de lect_sens al arreglo a publicar
  data_console.data = lect_sens;

  //Publica los datos de mi arreglo a mi topico
  publica.publish(&data_console);

  //Lo haga repetidamente
  nodito.spinOnce();
  delay(30);
}

