// PROGRAMA DE SENSORES ULTRASÓNICOS ROBOT DE PLAYA CON FILTRO DE MEDIA MÓVIL - POR: Adrián Ricárdez Ortigosa
// Cable naranja = +5V
// Cable azul = GND
// Cable blanco = Trigger
// Cable verde = Echo

// ----------------------------- VARIABLES GLOBALES Y DEFINICIONES ---------------------------------
// Cabe mencionar que se pueden usar los pines analógicos y digitales como entradas y salidas
const int u = 8;                                    // Número de sensores
const int TriggerPin[u] = {13,11,9,7,5,3,A0,A2};    // Pin de emergencia: sustituir alguno con A4
const int EchoPin[u] = {12,10,8,6,4,2,A1,A3};       // Pin de emergencia: sustituir alguno con A5
unsigned long Duration[u] = {0,0,0,0,0,0,0,0};      // La duración del Echo por el Trigger
long Distancia_mm[u] = {0,0,0,0,0,0,0,0};           // La distancia devuelta de la función fDistancia()

// Variables Filtro de Media Móvil {
const int f = 10;                                      // Unidades de filtro (MÁS GRANDE = MÁS SUAVIDAD EN EL FILTRO, MÁS PEQUEÑO = VALORES ACERCADOS AL REAL), recomendado: 10
int h = 0;                                             // Variable para comenzar la detección de los "f" valores de filtro
int rango_us = 5000;                                   // Rango recomendado para un metro de distancia: 6000
float Array_Ultra[u][f];                               // Arreglo de valores a filtrar
float Suma[u] = {0,0,0,0,0,0,0,0};        // Sumas del primer y segundo filtro
float Promedio[u] = {0,0,0,0,0,0,0,0};    // Promedios del primer y segundo filtro
boolean Primera_Fase = true;    // True = por lo menos una vez entrará a la primera fase, y luego nunca vuelve a entrar

// ====================================== VOID SETUP ================================================
void setup() {
  for(int i=0;i<u;i++)
  {
    pinMode(TriggerPin[i],OUTPUT);
    pinMode(EchoPin[i],INPUT);
  }
  Serial.begin(115200);
  Serial.begin(115200);
}

// ====================================== VOID LOOP =================================================
void loop() {  
  // FILTRO DE MEDIA MOVIL
  // ------------------------------------ SEGUNDA FASE DEL FILTRO ------------------------------------------------------------
  if( Primera_Fase == false ){
    // Corrimiento de valores obtenidos en la primera fase hacia la derecha, tipo queue
    for(int i=0;i<u;i++)     // De todos los ultrasónicos
    {
      // Restas del segundo filtro
      Suma[i] = Suma[i] - Array_Ultra[i][0];    // Para quitar el primer valor, el que se va a deshechar
      for(int j=0;j<(f-1);j++)   // f - 1 para que el Array_Ultra[i][f] tome el valor actual
      {
        Array_Ultra[i][j] = Array_Ultra[i][j+1];
      }
    }

    // La última posición toma el valor actual del respectivo ultrasónico
    for(int i=0;i<u;i++)
    {
      // Obtención de datos de los sonares
      digitalWrite(TriggerPin[i],HIGH);
      delayMicroseconds(10);
      digitalWrite(TriggerPin[i],LOW);
      Duration[i] = pulseIn(EchoPin[i], HIGH, rango_us);
      
      Distancia_mm[i] = fDistancia(Duration[i]);
      //Distancia_mm[i] = random(0,500);

      // De menor a mayor la distancia, para que no mande números tan bruscos
      for(int i=0;i<u;i++)
      {
        // Mientras más lejos está el objeto, más pequeño es el número (para que la respuesta del Mega sea suave)
        if(Distancia_mm[i] != 0)
          Distancia_mm[i] = 1000 - Distancia_mm[i];   // 1000 porque debe empezar a detectar aproximadamente desde esa distancia
      }
      
      Array_Ultra[i][f-1] = Distancia_mm[i];    // Toma el valor actual
      // Restas del segundo filtro
      Suma[i] = Suma[i] + Array_Ultra[i][f-1];    // Para agregar el último valor, el que también se va a promediar
    }

    // Promedios del segundo filtro
    for(int i=0;i<u;i++)
      Promedio[i] = Suma[i]/f;

    // Mapeo de cuadrantes según la función de Alex -1 -2 -3 3 2 1
    if(Promedio[0] >= 100 && Promedio[0] <= 1000)
      Promedio[0] = -1;
    else
      Promedio[0] = 0;
    if(Promedio[1] >= 100 && Promedio[1] <= 1000)
      Promedio[1] = -2;
    else
      Promedio[1] = 0;
    if(Promedio[2] >= 100 && Promedio[2] <= 1000)
      Promedio[2] = -3;
    else
      Promedio[2] = 0;
    if(Promedio[3] >= 100 && Promedio[3] <= 1000)
      Promedio[3] = 3;
    else
      Promedio[3] = 0;
    if(Promedio[4] >= 100 && Promedio[4] <= 1000)
      Promedio[4] = 2;
    else
      Promedio[4] = 0;
    if(Promedio[5] >= 100 && Promedio[5] <= 1000)
      Promedio[5] = 1;
    else
      Promedio[5] = 0;
      
    // Impresión de datos filtrados Monitor Serial
    // Envío de datos filtrados Arduino Mega
    Serial.print("s");
    Serial.print(Promedio[0]);
    Serial.print("a");
    Serial.print(Promedio[1]);
    Serial.print("b");
    Serial.print(Promedio[2]);
    Serial.print("c");
    Serial.print(Promedio[3]);
    Serial.print("d");
    Serial.print(Promedio[4]);
    Serial.print("e");
    Serial.print(Promedio[5]);
    Serial.print("f");
    Serial.print(Promedio[6]);
    Serial.print("g");
    Serial.print(Promedio[7]);
    Serial.print("h");
    Serial.println("$");

    // Reinicialización de promedios
    for(int i=0;i<u;i++)
      Promedio[i] = 0;
  }
  else{   // Si la bandera Primera_Fase == true
    // Guarda los primeros u valores de cada ultrasónico
    for(int j=0;j<f;j++)
    {
      for(int i=0;i<u;i++)
      {
        // Obtención de datos de los sonares
        digitalWrite(TriggerPin[i],HIGH);
        delayMicroseconds(10);
        digitalWrite(TriggerPin[i],LOW);
        Duration[i] = pulseIn(EchoPin[i], HIGH, rango_us);
        
        Distancia_mm[i] = fDistancia(Duration[i]);
        //Distancia_mm[i] = random(0,500);

        // De menor a mayor la distancia, para que no mande números tan bruscos
        for(int i=0;i<u;i++)
        {
          // Mientras más lejos está el objeto, más pequeño es el número (para que la respuesta del Mega sea suave)
          if(Distancia_mm[i] != 0)
            Distancia_mm[i] = 1000 - Distancia_mm[i];
        }
        
        Array_Ultra[i][j] = Distancia_mm[i];
      }
    }
    // -------------------------- PRIMERA FASE DEL FILTRO ------------------------------------------------------------- (SÓLO SE REALIZA 1 VEZ)
    // Suma del primer filtro
    for(int i=0;i<u;i++){
      for(int j=0;j<f;j++)
      {
        Suma[i] = Suma[i] + Array_Ultra[i][j];
      }
    }

    // Promedio del segundo filtro
    for(int i=0;i<u;i++)
      Promedio[i] = Suma[i]/f; // Promedio del primer filtro

    // Mapeo de cuadrantes según la función de Alex -1 -2 -3 3 2 1
    if(Promedio[0] >= 100 && Promedio[0] <= 1000)
      Promedio[0] = -1;
    else
      Promedio[0] = 0;
    if(Promedio[1] >= 100 && Promedio[1] <= 1000)
      Promedio[1] = -2;
    else
      Promedio[1] = 0;
    if(Promedio[2] >= 100 && Promedio[2] <= 1000)
      Promedio[2] = -3;
    else
      Promedio[2] = 0;
    if(Promedio[3] >= 100 && Promedio[3] <= 1000)
      Promedio[3] = 3;
    else
      Promedio[3] = 0;
    if(Promedio[4] >= 100 && Promedio[4] <= 1000)
      Promedio[4] = 2;
    else
      Promedio[4] = 0;
    if(Promedio[5] >= 100 && Promedio[5] <= 1000)
      Promedio[5] = 1;
    else
      Promedio[5] = 0;

    // Impresión de datos filtrados Monitor Serial
    // Envío de datos filtrados Arduino Mega
    Serial.print("s");
    Serial.print(Promedio[0]);
    Serial.print("a");
    Serial.print(Promedio[1]);
    Serial.print("b");
    Serial.print(Promedio[2]);
    Serial.print("c");
    Serial.print(Promedio[3]);
    Serial.print("d");
    Serial.print(Promedio[4]);
    Serial.print("e");
    Serial.print(Promedio[5]);
    Serial.print("f");
    Serial.print(Promedio[6]);
    Serial.print("g");
    Serial.print(Promedio[7]);
    Serial.print("h");
    Serial.println("$");

    // Reinicialización de promedios
    for(int i=0;i<u;i++)
      Promedio[i] = 0;
    
    Primera_Fase = false;   // Esto detiene que siga aumentando la h indefinidamente, hace que se pueda hacer la lectura de la última posición del arreglo
  }
}

// ---------------------------- FUNCIONES -------------------------------------------------------------------
long fDistancia(long tiempo) {
  // Calculamos la distancia en mm
  // ((tiempo)*(Velocidad del sonido)/ el camino se hace dos veces)

  long DistanceCalc; // Variable para los cálculos
  DistanceCalc = (tiempo / 2.9) / 2; // Cálculos en milímetros
  // DistanceCalc = (tiempo /29) / 2; // Cálculos en centímetros
  // DistanceCalc = (tiempo / 74) / 2; // Cálculos en pulgadas
  return DistanceCalc; // Devolvemos el calculo
}
