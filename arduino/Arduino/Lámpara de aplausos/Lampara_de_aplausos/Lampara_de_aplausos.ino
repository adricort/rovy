// PROGRAMA LÁMPARA/VELA POR: Σ
int sensorValue = 0;  // Variable para almacenar el valor del sensor de sonido
int A = 0;                    // Variable de apagado para ignorar 30 datos
int B = 0;                    // Variable de prendido para ignorar 30 datos
int aplausos = 0;           // Variable de aplausos
int tiempo = 0;             // Variable tiempo para reinicializar los aplausos
boolean efecto_de_funcion = false;  // Para empezar a contar A y B
boolean Reinicio_Aplausos = false;    // Activador de función reinicio de aplausos
boolean indicador = false;          // Indicador para decir si esta prendido o apagado
// PINES
const int Relevador = 11;     // Pin del Relevador
const int sensorPin = 7;      // Pin del Sensor
const int LED = 13;           // LED testigo

void setup(){
  pinMode(Relevador, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(sensorPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  sensorValue = !digitalRead(sensorPin);  // Lee los aplausos
  Serial.println(sensorValue);
  if(sensorValue)
    digitalWrite(LED,HIGH);
  else
    digitalWrite(LED,LOW);
  if( indicador == false ){
    if( sensorValue > 0){                // Activa la función de prendido para ignorar 30 datos
      efecto_de_funcion = true;
    }
    if( efecto_de_funcion == true ){
      B++;
    }
    if( B > 30 ){                        // Ignora 30 datos y prende
      aplausos++;
      B = 0;
      efecto_de_funcion = false;
      Reinicio_Aplausos = true;
    }
  }
  else{
    if( sensorValue > 0 ){              // Activa la función de apagado para ignorar 30 datos
      efecto_de_funcion = true;
    }
    if( efecto_de_funcion == true ){
      A++;
    }
    if( A > 30 ){                        // Ignora 30 datos y apaga
      aplausos++;
      A = 0;
      efecto_de_funcion = false;
      Reinicio_Aplausos = true;
    }
  }
  if( indicador == false && aplausos == 2 ){    // Prende Relevador
    indicador = true;
    digitalWrite(Relevador, HIGH);
    aplausos = 0;
    tiempo = 0;
    Reinicio_Aplausos = false;
  }
  if( indicador == true && aplausos == 2 ){    // Apaga Relevador
    indicador = false;
    digitalWrite(Relevador, LOW);
    aplausos = 0;
    tiempo = 0;
    Reinicio_Aplausos = false;
  }
  if( Reinicio_Aplausos == true ){      // Comienza a contar para reinicializar aplausos
    tiempo++; 
  }
  if( tiempo > 100 ){    // No se recibieron más aplausos, reinicializa los aplausos
    aplausos= 0;
    tiempo = 0;
    Reinicio_Aplausos = false;
  }
}
