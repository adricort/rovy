#include <VirtualWire.h> 

int Relevador = 12;

void setup() {
vw_setup(7000);        //Seleccionamos la velocidad de transmision de datos
vw_rx_start();         //Iniciamos la comunicación
pinMode(Relevador, OUTPUT);  //Asignamos la variable Relevador como salida
}

void loop(){
uint8_t msg[VW_MAX_MESSAGE_LEN];
uint8_t len = VW_MAX_MESSAGE_LEN;

if (vw_get_message(msg, &len)){  //Condicion para ver si hay mensaje
if ( msg[0] == 'E') {            //Si el mensaje es una E
digitalWrite(Relevador, HIGH);         //Encendemos el Relevador
}
else if (msg[0] == 'A'){         // Si es una A
digitalWrite(Relevador, LOW);        //Apagamos el Relevador
}
}
}
