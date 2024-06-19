// FUNCIÓN ANTIRREBOTE

const int tiempoAntirrebote= 10;

boolean antirrebote(int pin){
  int contador=0;
  boolean estado;    // Guarda el estado del botón.
  boolean estadoAnterior;    // Guarda el último estado del botón.
  
  do{
    estado= digitalRead(pin);
    if (estado != estadoAnterior){
      contador=0;
      estadoAnterior= estado;
    }
    else{
      contador= contador+1;
    }
    delay(1);
  }  while(contador < tiempoAntirrebote);
  return estado;
}

void setup(){
  Serial.begin(9600);
  pinMode(boton, INPUT);
}

void loop(){
  estadoBoton= digitalWrite(boton);
  if(estadoBoton != estadoBotonAnterior){
    if(rebote(boton)){
      cuenta++;
      Serial.println(cuenta);
    }
  }
  estadoBotonAnterior= estadoBoton;
}
