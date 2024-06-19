#include <VirtualWire.h>  //incluimos la libreria de virtualwire
int boton = 2;            //Asignamos el numero dos al boton
char *msg = "";           //Asignamos el mensaje en blanco
int eb = 0;               //Asignamos el estado del boton en 0

void setup(){
vw_setup(7000);            //Configuramos la velocidad de transimsion de datos
pinMode(boton, INPUT);    //Configuramos el pin boton como entrada
}

void loop () {
eb = digitalRead(boton);    //Leemos el estado del boton y lo guardamos en la variable
if ( eb == HIGH) {          //Condicion para ver si esta activado el boton
msg = "E";                //Si lo esta, asignamos la letra E al mensaje
vw_send((uint8_t *)msg, strlen(msg));  //y enviamos este mensaje
}
else {                      //Si no lo esta
msg = "A";                //Asignamos A al mensaje
vw_send((uint8_t *)msg, strlen(msg));    // y lo enviamos

}
}
