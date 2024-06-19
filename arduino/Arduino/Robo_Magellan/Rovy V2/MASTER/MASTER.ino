// Programa Arduino_Mega_MASTER - RoboMagellan 2018: Programa principal para la navegación y máquina de estados para ganar (Y) (Y) (Y)
// Nota: NO USAR "!!!" EN LOS SERIAL.PRINT, NO DEJA CARGAR EL PROGRAMA AL ARDUINO MEGA

// La numeración de todos los sensores y actuadores se definieron desde la perspectiva del robot, de izquierda a derecha, y de adelante hacia atrás
/* Valores simétricos máximos, mínimos y de interés:
    Servo izquierdo:    Servo derecho:    Camara neck:    Camara head:    PWM RoboClaw:   Encoders:
    Retraido: 25°        Retraido: 165°   Izquierda: 5°   Arriba: 100°    Mínimo: 0       D llanta: 0.12 [m]
    Extendido: 100°      Extendido: 85°   Centro: 96°     "Centro": 160°  Máximo: 127     1 vuelta enc: 1925 [u] (aproximadamente, +/-10)
                                          Derecha: 180°                                   Formula L recorrida: D*3.1416*1.2* (Lectura_Enc/1925) <--- es el n vueltas

   NOTA: La función Encoder() lee los datos de los encoders recorrida. Intentar usar cada cierto tiempo, no consecutivamente porque es costoso computacionalmente.
   Fórmula de distancia a recorrer entre punto y punto con base en coordenadas GPS:
                L_GPS = (sqrt((lat2-lat1)*pow(2)+(long2-long1)*pow(2)))*107100-2. Esa x*107100-2 la saqué experimentalmente de MAPS

   Rango de los PWM aproximadamente: [-60,120] con una k de 0.5 y velocidad crucero de alineacion de 30
*/

// Bibliotecas
#include <Servo.h>
#include <SoftwareSerial.h>
#include "RoboClaw.h"
#include <math.h>

// Direcciones packet serial RoboClaws
#define address0 0x80
#define address1 0x81
#define address2 0x82

// Multiplexeo de seriales
SoftwareSerial serialUNO(50, 52);

// Velocidad de los seriales
RoboClaw roboclaws(&serialUNO, 10000);

// Declaración de Motores 99:1 Pololu
Servo Camara_neck;    // Servo direccion cámara
Servo Camara_head;    // Servo base cámara
Servo Bandera_izq;    // Servo base cámara
Servo Bandera_der;    // Servo direccion cámara

// Declaración de pines
const int pin_neck = 26;
const int pin_head = 28;
const int pin_band_izq = 30;
const int pin_band_der = 32;
const int pin_STOP = 6;
const int pin_boton2 = 7;
const int pin_LED = 2;
const int pin_LED_calibration = 4;
const int pin_LED_nofix = 5;

// Variables Globales
double lat, lon, fix, angle, satel;
boolean primer_dato_IMU = false;    // Bandera para que reciba el primer dato anterior de la IMU sea el primero que recibe, sólo lo hace 1 vez
boolean bandera_pais = false;         // false = MEX, true = EU
boolean GPS_calibrado = false;        // Bandera que indica si ya se hizo la obtención del punto de origen
unsigned int n = 0;                                // Número de puntos objetivo
unsigned int p = 0;                       // Número que nos ayudará a saber qué alineacion/navegación toca entre qué puntos
unsigned long t = 0;
double latitudes[15];     // Crea el arreglo de coordenadas de los puntos objetivo en latitud, vamos a dejarlo en 15
double longitudes[15];    // Crea el arreglo de coordenadas de los puntos objetivo en longitud, vamos a dejarlo en 15
double latitudes_totales[15];   // Crea el arreglo de distancias totales del triángulo, este es el cateto opuesto
double longitudes_totales[15];  // Crea el arreglo de distancias totales del triángulo, este es el cateto adyacente
double L_GPS[15];         // Crea el arreglo de distancias, vamos a dejarlo en 15
boolean cono_o_punto[15]; // Crea el arreglo que nos confirma si es cono (1) o es un punto objetivo (0)
double latitud_inicial;
double longitud_inicial;
double alfa_radianes[15];
double alfa_grados[15];
long time_tocar[5] = {1150,1150,1150,1150,1150};
double error_angulo;         // Error del angulo para hacer la alineación y navegación
int eu = 0;                 // Error de los ultrasónicos (-1 -1 0 1 1)
unsigned long previousMillis = 0;     // Va a almacenar el último tiempo que ocurrió algo
unsigned long currentMillis = 0;          // Almacena la variable del tiempo actual
int PWM_izq = 0;
int PWM_der = 0;
long interval = 0;               // Intervalo de tiempo que simulará el "delay" sin detener procesos del arduino. Lo dejé en 0 ahorita para fines prácticos. No mover aquí,sólo en fórmulas!
boolean bandera_offsets_lado;   // Bandera que nos indicará a cual lado confiarle el offset
// Promedios de offsets para que el robot comience a calcular distancia desde que ya se alineó
double offsets_enc_izq = 0;
double offsets_enc_der = 0;
double offsets_enc = 0;
double L_enc_izq = 0;
double L_enc_der = 0;
unsigned int e_izq = 0;      // Variable local que nos ayudará a saber si se logró leer un encoder o no de la izquierda
unsigned int e_der = 0;      // Variable local que nos ayudará a saber si se logró leer un encoder o no de la derecha
long enc[6] = {0, 0, 0, 0, 0, 0};
long speeds[6] = {0, 0, 0, 0, 0, 0};
bool valid_enc[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
double offset_norte = 0;      // Offset de calibración
int Coordx_cono = 0;
long Saturacion = 0;
double IMU_angulo;
double angulo_obj_cono;

// Constantes Globales
const int u = 5;          // Número de ultrasónicos recibidos del Arduino UNO
const int min_band_izq = 15;
const int max_band_izq = 100;
const int min_band_der = 173;
const int max_band_der = 80;
const int min_neck = 15;
const int cent_neck = 100;
const int max_neck = 180;
const int max_head = 100;
const int cent_head = 153;
const int min_head = 180;
const int min_mot = 0;
const int max_mot = 127;
const int una_vuelta_enc = 1925;    // Cada vuelta de una llanta es el valor 1925 en el encoder
const float D_llanta = 0.12;
const float pi = 3.1416;
const long lat_cte_MEX = 1900000;       // Latitud constante en CDMX para fórmulas
const long long_cte_MEX = 9900000;      // Longitud constante en CDMX para fórmulas
const long lat_cte_EU = 3700000;        // Latitud constante en EU para fórmulas
const long long_cte_EU = 12100000;       // Longitud constante en EU para fórmulas
const float k_navegacion = 3.6;     // Constante que nos ayuda a suavizar el de la navegación
const int VC = 127;                 // Velocidad de crucero para la navegación
const float k_recorrido = 1.19;   // valor en estacionamiento 1.2//valor en casa 1.23        // Constante para el cálculo de la distancia recorrida
const int k_interval = 400;       // valor islas 450 // valor en casa 550       // Constante que nos determina cuánto tiempo debe esperarse el robot antes de abrir los ojos por primera vez para ver cuánto lleva recorrido.
const float Kx = 0.85;            //0.51 en mi casa    // Constante sobre todo para centrar el cono en X
const float Ky = 0.45;                // Constante para avanzar más rápido si el cono aún se encuentra muy lejos y bajar velocidad cuando ya esté cerca
const float k_L_GPS = 3;           //3.025// Constante para cálculo de la distancia a recorrer
const unsigned long veces_camara = 120000;      // Constante que nos indica cuandas veces va a buscar el cono la webcam antes de voltear a ver a otro lado
const long saturacion_minima = 80;     // Constante de valor mínimo para que el robot comience a detectar el cono
const long saturacion_maxima = 7300;   // Constante de valor máximo para que el robot toque el cono
const double K_saturacion = 0.0045;    // Constante para el cálculo de la potencia en función de la saturación
int h = 0;        // Contador que indica el número de veces que buscará el cono antes de usar el GPS cuando ya haya llegado a su destino
const int k_US = 60;
// CAMBIALAS A TU CONVENIENCIA, ES DONDE IDEALMENTE INICIA EL ROBOT
unsigned long origen_lat = 3739360;//inicio livermore//1918186;//1918196; inicio casa//1918186;  // coordenada estadio
unsigned long origen_long = 12153150;//inicio livermore//9909136;//9909170; inicio casa//9909136;  // coordenada estadio

#include "Lectura_US_IMU.h"
#include "Lectura_GPS.h"
#include "Lectura_Encoders.h"
#include "Lectura_WebCam.h"
#include "Actuadores.h"

// ====================================== VOID SETUP =================================================================
void setup() {
  // Deben estar todos en 115200
  Serial.begin(115200);     // Puerto serial para la comunicación con la computadora y datos GPS. Debe estar a 9600 por el GPS.
  Serial2.begin(115200);      // Puerto serial para la recepción de datos de los ultrasónicos e IMU.
  Serial1.begin(115200);      // Puerto serial para los datos de la cámara con la Raspberry Pi3

  roboclaws.begin(115200);

  pinMode(pin_STOP,INPUT);
  pinMode(pin_LED,OUTPUT);
  pinMode(pin_LED_nofix,OUTPUT);
  pinMode(pin_LED_calibration,OUTPUT);
  pinMode(pin_boton2,INPUT);

  // Posición inicial del robot
  originServos();
  mot(0, 0);
}

// ===================================================== VOID LOOP ============================================================
void loop() {
  goto INICIO;

  // --------------------------------------------------- INICIO: CONFIGURACIÓN DE NAVEGACIÓN -----------------------------------------------------------
INICIO:
  {
    Serial.println("CLUB DE ROBOTICA DE LA FACULTAD DE INGENIERÍA - ROBOGAMES 2018 - ROBOMAGELLAN - ROVY"); Serial.println();
    Serial.println("Ejemplos de coordenadas SIN PUNTO DECIMAL a ingresar en latitud y longitud:");
    Serial.println("37°39'32.7''N,121°53'14.1''W   ROBOGAMES 2018 PLEASANTON"); Serial.println();

    // ----------------------------- Configuración de Navegación ---------------------------------------------
    Serial.print("Ingrese la cantidad de puntos objetivo del robot, ejemplo '5': ");
    //while(Serial.available()==0){}  // Espera la entrada de la cantidad de puntos objetivo del usuario "n"
    //n = Serial.parseInt();      // La variable tomará el valor sólo de la parte de número entero que se ingrese
    n = 4;
    Serial.println(n);
    latitudes[n];     // Crea el arreglo de coordenadas de los puntos objetivo en latitud
    longitudes[n];    // Crea el arreglo de coordenadas de los puntos objetivo en longitud

    for (int i = 1; i <= n; i++) { // Ingreso de datos de orden n
      Serial.print("Ingrese la latitud en dato de la coordenada #"); Serial.print(i); Serial.print(" : ");
      //while(Serial.available()==0){}    // Espera la entrada de la cantidad de puntos objetivo del usuario
      //latitudes[i]= Serial.parseFloat();    // La variable tomará el valor sólo de la parte de número flotante que se ingrese
      latitudes[1] = 3739352;
      latitudes[2] = 3739357;//1918202; coladera casa//1918171; roca estadio
      latitudes[3] = 3739352;//1918212; esquina casa//1918166; cono estadio
      latitudes[4] = 3739361;//1918212; esquina casa//1918166; cono estadio
      
      Serial.println(latitudes[i] / 1000, 3);

      Serial.print("Ingrese la longitud de la coordenada #"); Serial.print(i); Serial.print(" : ");
      //while(Serial.available()==0){}
      //longitudes[i]= Serial.parseFloat(); // La variable tomará el valor sólo de la parte de número flotante que se ingrese
      longitudes[1] = 12153147;
      longitudes[2] = 12153127;//9909167; coladera casa//9909139; roca estadio
      longitudes[3] = 12153113;//9909164; esquina casa//9909125; cono estadio
      longitudes[4] = 12153113;//9909164; esquina casa//9909125; cono estadio
      Serial.println(longitudes[i] / 1000, 3);
    }
    Serial.println();

    for (int i = 0; i < n; i++)
    {
      Serial.print("El punto #"); Serial.print(i + 1); Serial.print(" es un cono o un punto? 1 = cono, 0 = punto: ");
      //while(Serial.available()==0){}
      //cono_o_punto[i] = Serial.parseInt();
      cono_o_punto[0] = 0;
      cono_o_punto[1] = 0;
      cono_o_punto[2] = 0;
      cono_o_punto[3] = 1;

      time_tocar[0] = 1150;
      
      Serial.println(cono_o_punto[i]);
    } Serial.println();

    Serial.println("El arreglo de confirmacion de conos o puntos es:");
    for (int i = 0; i < n; i++)
    {
      Serial.print(cono_o_punto[i]);
      Serial.print(" ");
    }
    Serial.println();

    if (latitudes[0] - lat_cte_EU > 100)
    {
      bandera_pais = false;
      Serial.println("Se encuentra en MEX");
      Serial.println("El arreglo de coordenadas objetivo de MEX en formato Google Earth WGS84 queda como:");
      for (int i = 1; i <= n; i++)
      {
        Serial.print("19°"); Serial.print((latitudes[i] - lat_cte_MEX) / 1000, 3); Serial.print("N,");
        Serial.print("99°"); Serial.print((longitudes[i] - long_cte_MEX) / 1000, 3); Serial.println("W");
      }
    }
    else
    {
      bandera_pais = true;
      Serial.println("Se encuentra en EU");
      Serial.println("El arreglo de coordenadas objetivo de EU en formato Google Earth WGS84 queda como:");
      for (int i = 1; i <= n; i++)
      {
        Serial.print("37°"); Serial.print((latitudes[i] - lat_cte_EU) / 1000, 3); Serial.print("N,");
        Serial.print("121°"); Serial.print((longitudes[i] - long_cte_EU) / 1000, 3); Serial.println("W");
      }
    }
    Serial.println();

    Serial.println("Posición auxiliar:");
    latitudes[0] = origen_lat;// Comentar esto despues, es de prueba
    longitudes[0] = origen_long;// Comentar esto despues, es de prueba
    Serial.print("37°"); Serial.print((latitudes[0] - lat_cte_EU) / 1000, 3); Serial.print("N,");
    Serial.print("121°"); Serial.print((longitudes[0] - long_cte_EU) / 1000, 3); Serial.println("W");
    //} //DESCOMENTAR*

    // Calibración de IMU
    digitalWrite(pin_LED_calibration,HIGH);
    delay(20000);    // Delay necesario para obtención de offset del Arduino Slave US_IMU
    for (int i = 0; i < 5; i++)
      offset_norte = 90 - leer_US_IMU(1);  // Obtención del offset, hay un error experimental de 3 grados
    Serial.print("Offset de la IMU obtenido al norte: "); Serial.println(offset_norte);
    digitalWrite(pin_LED_calibration,LOW);

    // Posición inicial de los servos
    originServos();

    Serial.println();

    Serial.println("GPS LISTO PARA CONECTAR...");

    // Se espera a que se oprima el botón para pasar a calcular todo
    while (true)
    {
      if (digitalRead(pin_boton2))
        goto CALCULO;
    }
  }

  // ---------------------------------------------- CÁLCULO: DE ÁNGULOS Y DISTANCIAS -----------------------------------------------------
CALCULO:
  {
    Serial.println("El arreglo de latitudes y longitudes totales queda como:");
    for (int i = 1; i <= n; i++)
    {
      latitudes_totales[i - 1] = latitudes[i] - latitudes[i - 1];
      longitudes_totales[i - 1] = (longitudes[i] - longitudes[i - 1]) * (-1); // Porque el dato es negativo
      Serial.print("Latitud total del punto "); Serial.print(i - 1); Serial.print(" al punto "); Serial.print(i); Serial.print(" es: "); Serial.println(latitudes_totales[i - 1] / 1000, 3);
      Serial.print("Longitud total del punto "); Serial.print(i - 1); Serial.print(" al punto "); Serial.print(i); Serial.print(" es: "); Serial.println(longitudes_totales[i - 1] / 1000, 3);
    }

    Serial.println();

    // Análisis de cuadrantes y transformación de radianes a grados
    Serial.println("El arreglo de angulos de direccion queda como:");
    for (int i = 0; i < n; i++)
    {
      alfa_radianes[i] = atan(latitudes_totales[i] / longitudes_totales[i]);
      alfa_grados[i] = alfa_radianes[i] * RAD_TO_DEG;  // Transformo los ángulos de dirección a grados

      // Esto para evitar indeterminaciones
      if(latitudes_totales[i] > 0 && longitudes_totales[i] == 0.0)
        alfa_grados[i] = 90.0;
      else if(latitudes_totales[i] < 0 && longitudes_totales[i] == 0.0)
        alfa_grados[i] = -270.0;
      Serial.print("Angulo-radianes del punto "); Serial.print(i); Serial.print(" al punto "); Serial.print(i + 1); Serial.print(" es: "); Serial.println(alfa_radianes[i], 3);
      Serial.print("Angulo-grados del punto "); Serial.print(i); Serial.print(" al punto "); Serial.print(i + 1); Serial.print(" es: "); Serial.println(alfa_grados[i], 3);
    }
    Serial.println();

    // Transformación a [0,360]
    for (int i = 0; i < n; i++) {
      // Cuadrante 1: 
      if ( longitudes_totales[i] >= 0 && latitudes_totales[i] >= 0)
      {
        Serial.print("El angulo "); Serial.print(i); Serial.println(" pertenece al primer cuadrante");
      }
      // Cuadrante 2:
      else if ( longitudes_totales[i] <= 0 && latitudes_totales[i] >= 0)
      {
        Serial.print("El angulo "); Serial.print(i); Serial.println(" pertenece al segundo cuadrante");
        alfa_grados[i] = 180 + alfa_grados[i];
      }
      // Cuadrante 3:
      else if ( longitudes_totales[i] <= 0 && latitudes_totales[i] <= 0)
      {
        Serial.print("El angulo "); Serial.print(i); Serial.println(" pertenece al tercer cuadrante");
        alfa_grados[i] = 180 + alfa_grados[i];
      }
      // Cuadrante 4:
      else if ( longitudes_totales[i] >= 0 && latitudes_totales[i] <= 0)
      {
        Serial.print("El angulo "); Serial.print(i); Serial.println(" pertenece al cuarto cuadrante");
        alfa_grados[i] = 360 + alfa_grados[i];
      }
    }

    Serial.println("");
    Serial.println("El arreglo de angulos de las triangulaciones de 0 a 360 es:");
    for (int i = 0; i < n; i++)
      Serial.println(alfa_grados[i], 3);

    Serial.println();

    // Cálculo de distancias a recorrer entre cada punto
    Serial.println("El arreglo de distancias a recorrer con los encoders queda como:");
    for (int i = 1; i <= n; i++)
    {
      L_GPS[i - 1] = sqrt(pow(latitudes[i] - latitudes[i - 1], 2) + pow(longitudes[i] - longitudes[i - 1], 2)) * k_L_GPS-1.3; // ejemplo: pow(2,4) = 2^4
      
      // Agregarle metros para que mientras más grande sea, recorra más distancia
      
      if(L_GPS[i-1] >= 15 && L_GPS[i-1] < 20)
        L_GPS[i-1] -= 0.3;
      else if(L_GPS[i-1] >= 20 && L_GPS[i-1] < 25)
        L_GPS[i-1] -= 0.7;
      else if(L_GPS[i-1] >= 25 && L_GPS[i-1] < 30)
        L_GPS[i-1] -= 1.3;
      else if(L_GPS[i-1] >= 30 && L_GPS[i-1] < 35)
        L_GPS[i-1] -= 2;
      else if(L_GPS[i-1] >= 35 && L_GPS[i-1] < 40)
        L_GPS[i-1] -= 2;
      else if(L_GPS[i-1] >= 40 && L_GPS[i-1] < 45)
        L_GPS[i-1] -= 2;
      else if(L_GPS[i-1] >= 45)
        L_GPS[i-1] -= 2;
        
      Serial.print("    Distancia a recorrer del punto "); Serial.print(i - 1); Serial.print(" al punto "); Serial.print(i); Serial.print(" es: "); Serial.print(L_GPS[i - 1], 6); Serial.println(" metros");
    } Serial.println();

    goto ALINEACION;
  }

  // ----------------------------------------------------- ALINEACION --------------------------------------------------------
ALINEACION:
  {
    Serial.println("Comenzando alineación..."); Serial.println();
    do
    {
      if(digitalRead(pin_STOP))
        goto STOP_ALINEACION;
      //while(Serial.available()==0);   // COMENTAR*
      //double IMU_angulo = Serial.parseFloat();    // COMENTAR*
      double IMU_angulo = leer_US_IMU(1); // DESCOMENTAR*

      // Para que, aún con el offset, siga en formato [0,360], por si al sumarle el offset se hace negativo el ángulo
      if (IMU_angulo + offset_norte < 0)
        IMU_angulo = IMU_angulo + offset_norte + 360;
      else
        IMU_angulo = IMU_angulo + offset_norte;

      Serial.print(" IMU [0,360]: "); Serial.print(IMU_angulo, 3); Serial.print(" alfa_grados [0,360]: "); Serial.print(alfa_grados[p], 3);

      // Calculo del error en función del mismo error. Ésta es la ecuación que me partió la cabeza.
      if ( (IMU_angulo - alfa_grados[p]) < 180)
        error_angulo = IMU_angulo - alfa_grados[p];
      else if ( (IMU_angulo - alfa_grados[p]) >= 180)
        error_angulo = (IMU_angulo - 360) - alfa_grados[p];

      // La llegada al ángulo de alineación más rápida, analizada por el menor error posible
      if (error_angulo >= 0) // gira a la derecha
      {
        PWM_izq = VC * 0.95;
        PWM_der = VC * 0.4;
      }
      else
      {
        PWM_izq = VC * 0.95;
        PWM_der = VC * 0.4;
      }
      mot(PWM_izq, PWM_der);  // Escribe en los motores

      Serial.print(" Error ang: "); Serial.print(error_angulo, 3); Serial.print(" PWM_izq: "); Serial.print(PWM_izq); Serial.print(" PWM_der: "); Serial.println(PWM_der);
    } while ( abs(error_angulo) > 20);      // Lo hará mientras el abs del error sea mayor a 5 (hasta que esté semi-alineado)

    Serial.println("Robot Alineado..."); Serial.println();

    // Delay para detención de motores para lectura de encoders
    mot(20, 20);
    delay(300);
    mot(0, 0);

    // Lectura de los offsets de los encoders
    Serial.println("Captura de offsets de encoders:");

    // Se hace esto hasta que se tenga una lectura fiable de los encoders
    do
    {
      leerEncoders();
    } while (!valid_enc[4] || !valid_enc[5] || enc[4] == 0  || enc[5] == 0);

    double offsets_enc_izq = enc[4];  // Son los que casi nunca fallan
    double offsets_enc_der = enc[5];  // Son los que casi nunca fallan

    if (offsets_enc_izq > offsets_enc_der)
    {
      offsets_enc = offsets_enc_izq;
      bandera_offsets_lado = true;
      Serial.print("Los motores para tomar medidas de distancia serán los izquierdos porque son los mas grandes: "); Serial.println(offsets_enc);
    }
    else if (offsets_enc_der > offsets_enc_izq)
    {
      offsets_enc = offsets_enc_der;
      bandera_offsets_lado = false;
      Serial.print("Los motores para tomar medidas de distancia serán los derechos porque son los mas grandes: "); Serial.println(offsets_enc);
    }

    Serial.print("Comenzando Navegacion del punto "); Serial.print(p); Serial.print(" al punto "); Serial.println(p + 1); Serial.println("...");

    currentMillis = millis();
    previousMillis = currentMillis;   // Para comenzar a contar en cuanto empiece a navegar
    // Fórmula del cálculo inicial del intervalo para saber mas o menos en cuánto debe "abrir los ojos" el robot para saber cuánto lleva recorrido
    interval = L_GPS[p] * k_interval;    // DESCOMENTAR*
    goto NAVEGACION;
  }

  // ----------------------------------------------------- NAVEGACIÓN --------------------------------------------------------
NAVEGACION:
  {
    if(digitalRead(pin_STOP))
      goto STOP_NAVEGACION;
    //while(Serial.available()==0);   // COMENTAR*
    //double IMU_angulo = 47.5;   // COMENTAR*
    //Serial.parseFloat();    // COMENTAR*
    double IMU_angulo = leer_US_IMU(1); // DESCOMENTAR*

    // Para que, aún con el offset, siga en formato [0,360]
    if (IMU_angulo + offset_norte < 0)
      IMU_angulo = IMU_angulo + offset_norte + 360;
    else
      IMU_angulo = IMU_angulo + offset_norte;

    // Calculo del error en función del mismo error. Esta es la ecuación que me partió la cabeza.
    if ( (IMU_angulo - alfa_grados[p]) < 180)
      error_angulo = IMU_angulo - alfa_grados[p];
    else if ( (IMU_angulo - alfa_grados[p]) >= 180)
      error_angulo = (IMU_angulo - 360) - alfa_grados[p];

    PWM_izq = VC + error_angulo * k_navegacion + leer_US_IMU(0)*k_US;
    PWM_der = VC - error_angulo * k_navegacion - leer_US_IMU(0)*k_US;

    // Estancamiento de valores
    if (PWM_izq > 127)
      PWM_izq = 127;
    else if (PWM_izq < 0)
      PWM_izq = 0;
    if (PWM_der > 127)
      PWM_der = 127;
    else if (PWM_der < 0)
      PWM_der = 0;

    mot(PWM_izq, PWM_der);  // Escribe en los motores

    // Rutina de evasión
    if(leer_US_IMU(2) == 1)
      goto RUTINA_EVASION;

    currentMillis = millis();

    Serial.print(" IMU [0,360]: "); Serial.print(IMU_angulo, 3); Serial.print(" alfa_grados [0,360]: "); Serial.print(alfa_grados[p], 3);
    Serial.print(" Error ang: "); Serial.print(error_angulo); Serial.print(" PWM_izq: "); Serial.print(PWM_izq); Serial.print(" PWM_der: "); Serial.print(PWM_der);
    Serial.print(" Intervalo de ceguera: "); Serial.println(interval);

    // Cada cierto tiempo se parará el robot para poder leer los encoders
    if ((currentMillis - previousMillis) >= interval)
    {
      Serial.println();
      Serial.println("Detencion de motores para leer encoders..."); Serial.println();
      mot(20, 20);
      delay(150);
      mot(0, 0);
      delay(100);
      Serial.print("Distancia GPS a recorrer: "); Serial.println(L_GPS[p]);

      double L_enc = 0;

      // Se hace esto hasta que se tenga una lectura fiable de los encoders
      do  //DESCOMENTAR*
      { //DESCOMENTAR*
        leerEncoders(); //DESCOMENTAR
      } while (!valid_enc[4] || !valid_enc[5] || enc[4] == 0 || enc[5] == 0); //DESCOMENTAR*


      long enc_izq_real;
      long enc_der_real;

      // Lectura real de los encoders, restándoles el offset
      if (offsets_enc <= 0)
      {
        enc_izq_real = enc[4] + offsets_enc;
        enc_der_real = enc[5] + offsets_enc;
      }
      else
      {
        enc_izq_real = enc[4] - offsets_enc;
        enc_der_real = enc[5] - offsets_enc;
      }

      Serial.println("El valor real del encoder recorrido desde el ultimo punto es:");
      if (bandera_offsets_lado)
      {
        Serial.print("Encoder real izquierdo: "); Serial.println(enc_izq_real);
      }
      else
      {
        Serial.print(" Encoder real derecho: "); Serial.println(enc_der_real);
      }
      Serial.println();

      L_enc_izq = D_llanta * pi * k_recorrido * (enc_izq_real / una_vuelta_enc);
      L_enc_der = D_llanta * pi * k_recorrido * (enc_der_real / una_vuelta_enc);

      // Lectura de la distancia recorrida (encoder positivo actual - offset encoder)
      if (bandera_offsets_lado)
        L_enc = L_enc_izq;
      else
        L_enc = L_enc_der;

      Serial.print("La distancia recorrida desde el ultimo punto al punto objetivo es: ");
      //while(Serial.available()==0){}      // COMENTAR*
      //L_enc = Serial.parseInt();     // COMENTAR*
      Serial.println(L_enc); Serial.println();

      if (L_enc >= L_GPS[p] || abs(L_enc - L_GPS[p]) <= 1.7 )  // Si ya llegó su destino o está por llegar
      {
        Serial.print("HE LLEGADO A MI DESTINOOOOOOOOOOOOOOOOOOOOOO"); Serial.println();
        
        // CHECAR SI DEJAR ESTO
        /*do
        {
          //Serial.println("Esperando lectura confiable del GPS...");
          digitalWrite(pin_LED_nofix,HIGH);
          leerGPS();  // DESCOMENTAR*
          leerGPS();
          leerGPS();
        } while (fix != 1 || satel < 7 || lat == 0 || lon == 0 || lat < 3700000 || lon < 12100000 || lat > 3799999 || lon > 12199999);  // leer el GPS mientras no se tenga una lectura confiable
        digitalWrite(pin_LED,HIGH);
        digitalWrite(pin_LED_nofix,LOW);
        if( abs(latitudes[p+1] - lat) > 1 || abs(longitudes[p+1] - lon) > 1)
          goto CALCULO_AUX;   */
        
        
        // Si el punto al que llegó es un cono
        if (cono_o_punto[p] == 1)
        {
          Serial.print("Comenzare a buscar un cono..."); Serial.println();
          goto BUSCAR_CONO;
        }
        // Si el punto al que llegó es un punto
        else
        {
          Serial.print("Llegue a un punto objetivo, iré al siguiente punto/cono ingresado"); Serial.println();
          p++;
          servos(min_band_izq, min_band_der, cent_neck, max_head, 300);
          originServos();
          if (p < n)  // Si ya acabé todos los puntos objetivo
          {
            //goto ALINEACION;
            do
            {
              //Serial.println("Esperando lectura confiable del GPS...");
              digitalWrite(pin_LED_nofix,HIGH);
              leerGPS();  // DESCOMENTAR*
              leerGPS();
              leerGPS();
            } while (fix != 1 || satel < 7 || lat == 0 || lon == 0 || lat < 3700000 || lon < 12100000 || lat > 3799999 || lon > 12199999);  // leer el GPS mientras no se tenga una lectura confiable
            digitalWrite(pin_LED,HIGH);
            digitalWrite(pin_LED_nofix,LOW);
            goto CALCULO_AUX;
          }
          else
            goto FINALIZACION;
        }
      }
      else        // Si no ha llegado a su destino
      {
        interval = (L_GPS[p] - L_enc) * k_interval;

        if ( (L_GPS[p] - L_enc) < 5 )  // Cuando le falten 7 metros para llegar
        {
          Serial.println("YA C-ASI LLEGO A MI DESTINO");
          interval += 900;
        }
        else
        {
          Serial.println("Aun no llego a mi destino, continuare navegando...");
        }
      } Serial.println();
      previousMillis = millis();
    }

    goto NAVEGACION;
  }

  // --------------------------------------------- RUTINA DE EVASIÓN CENTRAL ---------------------------------------
  RUTINA_EVASION:
  {
    offsets_enc += 5000;   // Valor aproximado que le aumenta al encoder para la rutina de evasión
    mot(0,0);
    delay(1000);
    mot(-VC*0.7,-VC*0.7);
    delay(1000);
    mot(0,0);
    delay(1000);
    if(leer_US_IMU(0) > 0)
    {
      // Forma una "S" hacia la izquierda
      mot(VC*0.2,VC*0.9);
      delay(1000);
      mot(VC*0.7,VC*0.7);
      delay(1000);
      mot(VC*0.9,VC*0.2);
      delay(1000);
      mot(0,0);
      goto NAVEGACION;
    }
    else
    {
      // Forma una "S" hacia la derecha
      mot(VC*0.9,VC*0.2);
      delay(800);
      mot(VC*0.7,VC*0.7);
      delay(800);
      mot(VC*0.2,VC*0.9);
      delay(800);
      mot(0,0);
      goto NAVEGACION;
    }
  }

  // -------------------------------------------------- BUSCAR CONO ----------------------------------------------------------
  BUSCAR_CONO:
  {
    if(digitalRead(pin_STOP))
      goto STOP_BUSCAR_CONO;
    digitalWrite(pin_LED, LOW);
    Serial.println("COMENZANDO A BUSCAR EL CONO CON LA WEBCAM...");
    originServos();
    mot(0, 0);

    // Búsqueda central. Hacer 50 veces para tener una lectura confiable
    for (long i = 0; i < veces_camara; i++)
      leerCamara();
    if (Coordx_cono != 0 && Saturacion >= saturacion_minima) // Si encuentra el cono en la posición original
    {
      Serial.println("Vaya vaya... parece que encontre el cono en el CENTRO.");
      Serial.println("Comenzando a acercar el cono...");
      goto ACERCAR_CONO;
      // CENTRAR_CONO;
    }

    // Si no encuentra el cono en la posición original gira a la izquierda
    servos(min_band_izq, min_band_der, min_neck, cent_head, 1100);

    // Búsqueda izquierda. Hacer 50 veces para tener una lectura confiable
    for (long i = 0; i < veces_camara; i++)
      leerCamara();
    if (Coordx_cono != 0 && Saturacion >= saturacion_minima )   // Si encuentra el cono en la posición izquierda
    {
      Serial.print("Vaya vaya... parece que encontre el cono a mi IZQUIERDA.");
      originServos();
      Serial.println("Comenzando a girar a la izquierda...");
      currentMillis = millis();
      previousMillis = currentMillis;
      goto GIRO_CONO_IZQ;   // DESCOMENTAR*
    }

    // Si no encuentra el cono en la posición izquierda gira a la derecha
    servos(min_band_izq, min_band_der, max_neck, cent_head, 2200);

    // Búsqueda derecha. Hacer 50 veces para tener una lectura confiable
    for (long i = 0; i < veces_camara; i++)
      leerCamara();
    if (Coordx_cono != 0 && Saturacion >= saturacion_minima)   // Si encuentra el cono en la posición derecha
    {
      Serial.print("Vaya vaya... parece que encontre el cono a mi DERECHA.");
      originServos();
      Serial.println("Comenzando a girar a la derecha...");
      currentMillis = millis();
      previousMillis = currentMillis;
      goto GIRO_CONO_DER;   // DESCOMENTAR*
    }

    // Si no pudo encontrarlo en ninguna de las 3 posiciones...
    Serial.println("No pude encontrar el cono en ningun lado... =(");
    servos(min_band_izq, min_band_der, cent_neck, cent_head, 200);
    servos(min_band_izq, min_band_der, cent_neck, min_head, 200);
    servos(min_band_izq, min_band_der, cent_neck, cent_head, 200);


    // Para que busque la webcam por lo menos 3 veces girando el robot, antes de usar el GPS
    if (h < 2)
    {
      mot(-VC*0.9,-VC*0.3);
      delay(1500);
      mot(-20, -20);
      delay(200);
      mot(0, 0);
      mot(VC*0.3, VC * 0.9);
      delay(1500);
      mot(20, 20);
      delay(200);
      mot(0, 0);
      h++;
      goto BUSCAR_CONO;
    }

    // Como no encontró el cono en ninguna de las posiciones, se procederá a usar el GPS para re-calcular el ángulo de giro y la distancia que hay que recorrer
    Serial.println("Utilizare el GPS para ubicarme de nuevo"); Serial.println();
    //goto BUSCAR_CONO;   // COMENTAR*

    h = 0;    // Reinicialización de variable
    do
    {
      //Serial.println("Esperando lectura confiable del GPS...");
      digitalWrite(pin_LED_nofix,HIGH);
      leerGPS();  // DESCOMENTAR*
      leerGPS();
      leerGPS();
    } while (fix != 1 || satel < 7 || lat == 0 || lon == 0 || lat < 3700000 || lon < 12100000 || lat > 3799999 || lon > 12199999 );  // leer el GPS mientras no se tenga una lectura confiable

    digitalWrite(pin_LED_nofix,LOW);
    digitalWrite(pin_LED, HIGH);  // Significa que ya obtuvo coordenadas del GPS

    Serial.print("Mi latitud actual es: "); Serial.print(lat / 1000, 3); Serial.print(" Mi longitud actual es: "); Serial.println(lon / 1000, 3);
    goto CALCULO_AUX;
  }
  // ----------------------------------------------- CENTRA EL CONO CON LA CÁMARA ----------------------------------------------------
  /*CENTRAR_CONO:
    {
    // Para obtener una lectura fiable
    for(int i = 0; i < 50; i++)
      leerCamara();

    int angulo_centrado_neck;
    int angulo_centrado_neck_anterior;

    Serial.println("Centrando...");

    if(Coordx_cono != 0 && Saturacion > saturacion_minima)
    {
      angulo_centrado_neck = cent_neck + (Coordx_cono-160)*0.12;

      // Control de centrado con motores
      if(angulo_centrado_neck <= cent_neck)
      {
        PWM_izq = (Coordx_cono - 160)*Kx - (cent_neck - angulo_centrado_neck);   // Función sacada por análisis de cuadrantes y valores
        PWM_der = -PWM_izq;   // Función sacada por análisis de cuadrantes y valores
      }
      else
      {
        PWM_izq = (Coordx_cono - 160)*Kx + (angulo_centrado_neck - cent_neck);   // Función sacada por análisis de cuadrantes y valores
        PWM_der = -PWM_izq;   // Función sacada por análisis de cuadrantes y valores
      }

      angulo_centrado_neck_anterior = angulo_centrado_neck;
    }
    else
    {
      if(angulo_centrado_neck_anterior <= cent_neck)
      {
        PWM_izq = -VC/2;
        PWM_der = VC/2;
      }
      else
      {
        PWM_izq = VC/2;
        PWM_der = -VC/2;
      }
    }

    // Estancamiento de valores
    if(PWM_izq > 90)
      PWM_izq = 90;
    else if(PWM_izq < -90)
      PWM_izq = -90;
    if(PWM_der > 90)
      PWM_der = 90;
    else if(PWM_der < -90)
      PWM_der = -90;

    if(angulo_centrado_neck > 180)
      angulo_centrado_neck = 180;
    else if(angulo_centrado_neck < 5)
      angulo_centrado_neck = 5;

    // Control de centrado con neck
    Camara_neck.attach(pin_neck);
    Camara_neck.write(angulo_centrado_neck);
    mot(PWM_izq,PWM_der);
    delay(70);      // Delay necesario ya que es parte del comportamiento del robot en centrar el cono
    Camara_neck.detach();
    mot(PWM_izq/4,PWM_der/4);

    if( (Coordx_cono - 160) < 50 && (Coordx_cono - 160) > -50 )   // Si ya está centrado
    {
      Serial.println("CONO CENTRADO Y LISTO PARA TOCAR");
      mot(0,0);
      delay(500);
      goto ACERCAR_CONO;
    }

    goto CENTRAR_CONO;
    }*/

  // ------------------------------------------------ RUTINA TOCAR CONO ---------------------------------------------------------------
ACERCAR_CONO:
  {
    if(digitalRead(pin_STOP))
      goto STOP_ACERCAR_CONO;
    leerCamara();
    leerCamara();
    leerCamara();
    if (Coordx_cono != 0 && Saturacion > saturacion_minima)
    {
      PWM_izq = (VC * 0.75) + (Coordx_cono - 160) * Kx - Saturacion * K_saturacion;
      PWM_der = (VC * 0.75) - (Coordx_cono - 160) * Kx - Saturacion * K_saturacion;
    }
    else
    {
      PWM_izq = VC * 0.75;
      PWM_der = VC * 0.75;
      mot(PWM_izq, PWM_der);
      delay(500);
      goto BUSCAR_CONO;
    }

    // Estancamiento de valores
    if (PWM_izq > 100)
      PWM_izq = 100;
    else if (PWM_izq < 0)
      PWM_izq = 0;
    if (PWM_der > 100)
      PWM_der = 100;
    else if (PWM_der < 0)
      PWM_der = 0;

    /*Serial.print("PWM_izq: ");Serial.print(PWM_izq);
      Serial.print("  PWM_der: ");Serial.println(PWM_der);*/

    if (Saturacion >= saturacion_maxima)
    {
      mot(0, 0);
      servos(min_band_izq, min_band_der, cent_neck, max_head, 500);
      originServos();
      Serial.println("RUTINA TOCAR CONO");
      goto TOCAR_CONO;
    }

    mot(PWM_izq, PWM_der);

    goto ACERCAR_CONO;
  }

  // ------------------------------------------------ RUTINA: TOCAR CONO --------------------------------------------------
TOCAR_CONO:
  {
    for(long i = 0; i < veces_camara; i++)
      leerCamara();
      
    servos(max_band_izq, max_band_der, cent_neck, cent_head, 300);
    mot(VC*0.6,VC*0.6);
    delay(time_tocar[t]);    // 1200 pasto, 1100 para pavimento
    mot(0,0);
    delay(300);
    mot(-VC*0.6,-VC*0.6);
    delay(time_tocar[t] + 300);
    mot(0,0);
    delay(200);
    p++;
    t++;
    if (p < n)  // Si todavía le faltan puntos por recorrer
      goto ALINEACION;
    else
      goto FINALIZACION;
  }

  // ----------------------------------------------- GIRO IZQUIERDO DE BÚSQUEDA DE CONO -------------------------------------------------
  GIRO_CONO_IZQ:
  {
    currentMillis = millis();
    mot(VC * 0.3, VC * 0.8); // Giro a la izquierda
    //Serial.print("currentMillis: ");Serial.print(currentMillis);
    //Serial.print(" previousMillis: ");Serial.println(previousMillis);
    if (currentMillis - previousMillis > 1500)
    {
      mot(0,0);
      delay(100);
      previousMillis = currentMillis;
      goto GIRO_CONO_IZQ2;
    }

    leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara();
    if (Coordx_cono != 0 && Saturacion >= saturacion_minima)   // Si encuentra el cono en la posición derecha
    {
      Serial.println("Cono encontrado al frente una vez mas...");
      goto BUSCAR_CONO;
    }

    goto GIRO_CONO_IZQ;
  }
GIRO_CONO_IZQ2:
  {
    mot(-VC * 0.8, -VC * 0.3);
    currentMillis = millis();
    //Serial.print("currentMillis: ");Serial.print(currentMillis);
    //Serial.print(" previousMillis: ");Serial.println(previousMillis);
    if (currentMillis - previousMillis > 1500)
    {
      mot(0,0);
      delay(100);
      previousMillis = currentMillis;
      goto GIRO_CONO_IZQ;
    }

    leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara();
    if (Coordx_cono != 0 && Saturacion >= saturacion_minima)   // Si encuentra el cono en la posición derecha
    {
      Serial.println("Cono encontrado al frente una vez mas...");
      goto BUSCAR_CONO;
    }

    goto GIRO_CONO_IZQ2;
  }

  // ----------------------------------------------- GIRO DERECHA DE BÚSQUEDA DE CONO -------------------------------------------------
GIRO_CONO_DER:
  {
    mot(VC * 0.8, VC * 0.3); // Giro a la izquierda
    currentMillis = millis();
    //Serial.print("currentMillis: ");Serial.print(currentMillis);
    //Serial.print(" previousMillis: ");Serial.println(previousMillis);
    if (currentMillis - previousMillis > 1500)
    {
      mot(0,0);
      delay(100);
      previousMillis = currentMillis;
      goto GIRO_CONO_DER2;
    }

    leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara();
    if (Coordx_cono != 0 && Saturacion >= saturacion_minima)   // Si encuentra el cono en la posición derecha
    {
      Serial.println("Cono encontrado al frente una vez mas...");
      goto BUSCAR_CONO;
    }

    goto GIRO_CONO_DER;
  }
GIRO_CONO_DER2:
  {
    mot(-VC * 0.3, -VC * 0.8);
    currentMillis = millis();
    //Serial.print("currentMillis: ");Serial.print(currentMillis);
    //Serial.print(" previousMillis: ");Serial.println(previousMillis);
    if (currentMillis - previousMillis > 1500)
    {
      mot(0,0);
      delay(100);
      previousMillis = currentMillis;
      goto GIRO_CONO_DER;
    }

    leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara(); leerCamara();
    if (Coordx_cono != 0 && Saturacion >= saturacion_minima)   // Si encuentra el cono en la posición derecha
    {
      Serial.println("Cono encontrado al frente una vez mas...");
      goto BUSCAR_CONO;
    }

    goto GIRO_CONO_DER2;
  }

  // ----------------------------------------------- CALCULO AUXILIAR POR SI EL ROBOT SE PIERDE -------------------------------------------------
CALCULO_AUX:
  {
    latitudes_totales[p] = latitudes[p + 1] - lat;
    longitudes_totales[p] = (longitudes[p + 1] - lon) * (-1);
    Serial.println();
    Serial.print("La latitud total de la posicion actual al punto "); Serial.print(p + 1); Serial.print(" es: "); Serial.println(latitudes_totales[p] / 1000, 3);
    Serial.print("La longitud total de la posicion actual al punto "); Serial.print(p + 1); Serial.print(" es: "); Serial.println(longitudes_totales[p] / 1000, 3);
    Serial.println();

    // Análisis de cuadrante y transformación de radianes a grados
    alfa_radianes[p] = atan(latitudes_totales[p] / longitudes_totales[p]);
    alfa_grados[p] = alfa_radianes[p] * RAD_TO_DEG;
    
    // Esto para evitar indeterminaciones
    if(latitudes_totales[p] > 0 && longitudes_totales[p] == 0.0)
      alfa_grados[p] = 90.0;
    else if(latitudes_totales[p] < 0 && longitudes_totales[p] == 0.0)
      alfa_grados[p] = -270.0;
      
    // Transformación a [0,360]
    // Cuadrante 1
    if ( longitudes_totales[p] >= 0 && latitudes_totales[p] >= 0)
      Serial.println("El angulo auxiliar pertenece al primer cuadrante");
    // Cuadrante 2
    else if ( longitudes_totales[p] <= 0 && latitudes_totales[p] >= 0)
    {
      Serial.println("El angulo auxiliar pertenece al segundo cuadrante");
      alfa_grados[p] = 180 + alfa_grados[p];
    }
    // Cuadrante 3:
    else if ( longitudes_totales[p] <= 0 && latitudes_totales[p] <= 0)
    {
      Serial.println("El angulo auxiliar pertenece al tercer cuadrante");
      alfa_grados[p] = 180 + alfa_grados[p];
    }
    // Cuadrante 4:
    else if ( longitudes_totales[p] >= 0 && latitudes_totales[p] <= 0)
    {
      Serial.println("El angulo auxiliar pertenece al cuarto cuadrante");
      alfa_grados[p] = 360 + alfa_grados[p];
    }
    Serial.print("El angulo auxiliar en radianes es: "); Serial.print(alfa_radianes[p], 3); Serial.print(" . Y en formato 360 grados es: "); Serial.println(alfa_grados[p], 3); Serial.println();


    // Cálculo de distancia a recorrer de la posición actual y el cono buscado y no encontrado
    L_GPS[p] = sqrt(pow(latitudes_totales[p], 2) + pow(longitudes_totales[p], 2)) * k_L_GPS -1.3; // ejemplo: pow(2,4) = 2^4

      // Agregarle metros para que mientras más grande sea, recorra más distancia
      
      if(L_GPS[p] >= 15 && L_GPS[p] < 20)
        L_GPS[p] -= 0.3;
      else if(L_GPS[p] >= 20 && L_GPS[p] < 25)
        L_GPS[p] -= 0.7;
      else if(L_GPS[p] >= 25 && L_GPS[p] < 30)
        L_GPS[p] -= 1.3;
      else if(L_GPS[p] >= 30 && L_GPS[p] < 35)
        L_GPS[p] -= 2;
      else if(L_GPS[p] >= 35 && L_GPS[p] < 40)
        L_GPS[p] -= 3;
      else if(L_GPS[p] >= 40 && L_GPS[p] < 45)
        L_GPS[p] -= 3;
      else if(L_GPS[p] >= 45)
        L_GPS[p] -= 3;


    Serial.print("La distancia a recorrer de la posicion actual al punto "); Serial.print(p + 1); Serial.print(" es: "); Serial.print(L_GPS[p], 6); Serial.println(" metros");

    goto ALINEACION;
  }


  // ------------------------------------------------- FINALIZACION --------------------------------------------------
  FINALIZACION:
  {
    Serial.println("HE TERMINADO MI BUSQUEDA... ESPERO HABERTE DADO UN BUEN VIAJE. GRACIAS CROFI-UNAM");
    mot(0, 0);
    originServos();
    servos(min_band_izq, min_band_der, cent_neck, max_head, 300);
    servos(min_band_izq, min_band_der, cent_neck, min_head, 300);
    servos(min_band_izq, min_band_der, cent_neck, max_head, 300);
    originServos();
    goto INICIO;
  }

  STOP_NAVEGACION:
  {
    Serial.println("STOP MANUAL ACTIVADO");
    mot(0,0);
    if(!digitalRead(pin_STOP))
      goto NAVEGACION;
    goto STOP_NAVEGACION;
  }

  STOP_ACERCAR_CONO:
  {
    Serial.println("STOP MANUAL ACTIVADO");
    mot(0,0);
    if(!digitalRead(pin_STOP))
      goto ACERCAR_CONO;
    goto STOP_ACERCAR_CONO;
  }

  STOP_BUSCAR_CONO:
  {
    Serial.println("STOP MANUAL ACTIVADO");
    mot(0,0);
    if(!digitalRead(pin_STOP))
      goto BUSCAR_CONO;
    goto STOP_BUSCAR_CONO;
  }

  STOP_ALINEACION:
  {
    Serial.println("STOP MANUAL ACTIVADO");
    mot(0,0);
    if(!digitalRead(pin_STOP))
      goto ALINEACION;
    goto STOP_ALINEACION;
  }
}
