#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

Servo servo_volumen,servo_ganancia;

int servo_volumen_angulo = 90;  // cadera derecha SI
int servo_ganancia_angulo = 90;   // cadera izquierda SI
boolean boton_valor = false;

int volumen_anterior;
int ganancia_anterior;

int protocolo1 = 0;
int protocolo2 = 0;
int protocolo3 = 0;

void setup() {
   servo_volumen.attach(5);
   servo_ganancia.attach(9); 
  
  Serial.begin(38400); // Default communication rate of the Bluetooth module
  mySerial.begin(38400); // Default communication rate of the Bluetooth module
}

void loop() {
  if(mySerial.available() > 0)
  {
    String PWMs = mySerial.readStringUntil(36); // 36 = $
    
    int indice_0 = PWMs.lastIndexOf('s');
    int indice_1 = PWMs.indexOf('F',indice_0+1);
    int indice_2 = PWMs.indexOf('a',indice_1+1);
    int indice_3 = PWMs.indexOf('G',indice_2+1);
    int indice_4 = PWMs.indexOf('b',indice_3+1);
    int indice_5 = PWMs.indexOf('H',indice_4+1);
    int indice_6 = PWMs.indexOf('c',indice_5+1);
    
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = PWMs.substring(indice_0+1,indice_1);
    String cadena_2 = PWMs.substring(indice_1+1,indice_2);
    String cadena_3 = PWMs.substring(indice_2+1,indice_3);
    String cadena_4 = PWMs.substring(indice_3+1,indice_4);
    String cadena_5 = PWMs.substring(indice_4+1,indice_5);
    String cadena_6 = PWMs.substring(indice_5+1,indice_6);
    
    // Parse.Int de la cadena
    servo_volumen_angulo = cadena_1.toInt();
    protocolo1 = cadena_2.toInt();
    servo_ganancia_angulo = cadena_3.toInt();
    protocolo2 = cadena_4.toInt();
    boton_valor = cadena_5.toInt();
    protocolo3 = cadena_6.toInt();

    // SUPER FILTRO RX/TX
    if(protocolo1 == 999 && protocolo2 == 888 && protocolo3 == 777)
      if(servo_volumen_angulo != 999 && servo_volumen_angulo != 888 && servo_volumen_angulo != 777 && servo_volumen_angulo <= 180 && servo_volumen_angulo >= 0)
        if(servo_ganancia_angulo != 999 && servo_ganancia_angulo != 888 && servo_ganancia_angulo != 777 && servo_ganancia_angulo <= 180 && servo_ganancia_angulo >= 0)
          if(boton_valor != 999 && boton_valor != 888 && boton_valor != 777)
            if((abs(servo_volumen_angulo - volumen_anterior) < 10) && (abs(servo_ganancia_angulo - ganancia_anterior) < 10))
            {
              // Controlando el servo motor
              servo_volumen.write(servo_volumen_angulo);
              Serial.print("Volumen: ");Serial.print(servo_volumen_angulo);
              Serial.print(" Protocolo1: ");Serial.print(protocolo1);
              Serial.print(" Ganancia: ");Serial.print(servo_ganancia_angulo);
              Serial.print(" Protocolo2: ");Serial.print(protocolo2);
              Serial.print(" Distor: ");Serial.print(boton_valor);
              Serial.print(" Protocolo3: ");Serial.println(protocolo3);
            }
    volumen_anterior = servo_volumen_angulo;
    ganancia_anterior = servo_ganancia_angulo;
  }
}
