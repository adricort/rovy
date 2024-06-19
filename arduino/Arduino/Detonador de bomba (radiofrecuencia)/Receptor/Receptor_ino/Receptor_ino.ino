#include <VirtualWire.h>  //incluimos la libreria de virtualwire

int led = 12; //Asignamos el 12 a la variable led

void setup() {
vw_setup(7000);        //Seleccionamos la velocidad de transmision de datos
vw_rx_start();         //Iniciamos la comunicación
pinMode(led, OUTPUT);  //Asignamos la variable led como salida
}

void loop(){
uint8_t msg[VW_MAX_MESSAGE_LEN];
uint8_t len = VW_MAX_MESSAGE_LEN;

if (vw_get_message(msg, &len)){  //Condicion para ver si hay mensaje
if ( msg[0] == 'E') {            //Si el mensaje es una E
digitalWrite(led, HIGH);         //Encendemos el LED
}
else if (msg[0] == 'A'){         // Si es una A
digitalWrite(led, LOW);        //Apagamos el led
}
}
}
