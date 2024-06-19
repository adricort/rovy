#include <ros.h>  
#include <std_msgs/Float32MultiArray.h>

#define PWM_traccion_pin 3
#define PWM_direccion_pin 5

#define Dir_adel_pin 8
#define Dir_atr_pin 9
#define Dir_izq_pin 10
#define Dir_der_pin 11

// ---------------------- VARIABLES -----------------------------
int PWM_traccion = 0;
int PWM_direccion = 0;

// Crea el arreglo de datos de los motores
float motores[2] = {0,0};   // {PWM_traccion,PWM_direccion}

// Banderas para los cambios de dirección de traccion y direccion
boolean bandera_dir_adel = false;
boolean bandera_dir_atr = false;
boolean bandera_dir_izq = false;
boolean bandera_dir_der = false;
// ---------------------------------------------------------------

ros::NodeHandle nh;

// Suscriptor
void messageCb(const std_msgs::Float32MultiArray& potencia_msg){
  PWM_traccion = (int)potencia_msg.data[0];
  PWM_direccion = (int)potencia_msg.data[1];

  // Estancamiento de valores tracción
  if(PWM_traccion > 255)
    PWM_traccion = 255;
  else if(PWM_traccion < -255)
    PWM_traccion = -255;

  // Estancamiento de valores dirección
  if(PWM_direccion > 150)
    PWM_direccion = 150;
  else if(PWM_direccion < -150)
    PWM_direccion = -150;

  // motores[i] es una variable aparte para poder ver en rostopic lo que nos despliega en un publicador
  motores[0] = PWM_traccion;
  motores[1] = PWM_direccion;

  // Conversión a datos útiles en traccion y cambio de reversa
  if(PWM_traccion < 0)
  {
    PWM_traccion = abs(PWM_traccion);
    bandera_dir_adel = true;
    bandera_dir_atr = false;
  }
  else
  {
    bandera_dir_adel = false;
    bandera_dir_atr = true;
  }

  // Conversión a datos útiles en direccion y cambios de giro
  if(PWM_direccion < 0)
  {
    PWM_direccion = abs(PWM_direccion);
    bandera_dir_izq = false;
    bandera_dir_der = true;
  }
  else
  {
    bandera_dir_izq = true;
    bandera_dir_der = false;
  }
}

std_msgs::Float32MultiArray data_console;
ros::Subscriber<std_msgs::Float32MultiArray> sub("/motores_topic", &messageCb );
ros::Publisher pub("/motores_listener", &data_console);

// ============================================ VOID SETUP ===============================================
void setup()
{
  pinMode(PWM_traccion_pin, OUTPUT);
  pinMode(PWM_direccion_pin, OUTPUT);
  pinMode(Dir_adel_pin, OUTPUT);
  pinMode(Dir_atr_pin, OUTPUT);
  pinMode(Dir_izq_pin, OUTPUT);
  pinMode(Dir_der_pin, OUTPUT);
  nh.initNode();
  nh.advertise(pub);
  nh.subscribe(sub);

  //Declara tamaño del arreglo a publicar
  data_console.data_length = 2;
}


// ============================================ VOID LOOP ===============================================
void loop()
{
  analogWrite(PWM_traccion_pin,PWM_traccion);
  analogWrite(PWM_direccion_pin,PWM_direccion);
  digitalWrite(Dir_adel_pin,bandera_dir_adel);
  digitalWrite(Dir_atr_pin,bandera_dir_atr);
  digitalWrite(Dir_izq_pin,bandera_dir_izq);
  digitalWrite(Dir_der_pin,bandera_dir_der);
  
  // Paso datos de lo que estoy escribiendo en los motores al arreglo a publicar para ROS
  data_console.data = motores;
  
  pub.publish( &data_console );
  nh.spinOnce();
  delay(1);
}
