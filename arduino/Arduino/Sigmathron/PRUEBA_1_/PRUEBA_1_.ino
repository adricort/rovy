const byte potPin1 = A0;
const byte potPin2 = A1;// terminal de la señal del potenciometro

const byte PWMpin1 = 3;// terminal de la señal PWM

const byte IN1pin = 4; // terminal a la entrada IN1

const byte IN2pin = 5;

const byte PWMpin2 = 6;

const byte IN3pin = 7;

const byte IN4pin = 9;// terminal a la entrada IN2

// Variables globales

int potValue1;
int potValue2;

int PWMvalue1;
int PWMvalue2;

// Configuración de terminales digitales

void setup (){

// establece las terminales IN1 e IN2 como salidas

pinMode (IN1pin, OUTPUT);

pinMode (IN2pin, OUTPUT);

pinMode (IN3pin, OUTPUT);

pinMode (IN4pin, OUTPUT);

}

void loop (){

// Lee el valor del potenciometro

potValue1 = analogRead (potPin1);
potValue2 = analogRead (potPin2);

// Realiza el “mapeo” del valor leído del potenciometro

PWMvalue1 = potValue1;
PWMvalue2 = potValue2;

// Salida de la señal PWM por la terminal PWMpin

analogWrite (PWMpin1, PWMvalue1);
analogWrite (PWMpin2, PWMvalue2);

// Establece el sentido de giro del motor

if (potValue1 >= 212){

 digitalWrite (IN1pin, LOW);
 digitalWrite (IN2pin, HIGH);

}
if (potValue2 >= 212){

 digitalWrite (IN4pin, LOW);
 digitalWrite (IN3pin, HIGH);
}

}
