// BIBLIOTECAS
#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial que multiplexa puertos seriales 
SoftwareSerial BT(10,11);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth

// VOID SETUP (CONFIGURACION)
void setup()
{
  // baudrate = tasa de datos que manda (9600,38400,115200)
  BT.begin(38400);       // Inicializamos el puerto serie BT (Para Modo AT 2) de fábrica es 38400
  Serial.begin(38400);   // Inicializamos el puerto serie con la computadora y el Arduino  
}

// VOID LOOP (CICLO DEL PROGRAMA)
void loop()
{
  // Transmisión de datos
  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    Serial.write(BT.read());
  }
 
  if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
     BT.write(Serial.read());
  }
}

// Pasos:
// MODO AT 2 (apretando boton, antes de energizar)
