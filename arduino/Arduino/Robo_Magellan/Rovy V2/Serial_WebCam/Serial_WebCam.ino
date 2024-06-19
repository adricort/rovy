// ====================================== VOID SETUP =================================================================
void setup() {
  Serial.begin(57600);     // Puerto serial para la comunicación con la computadora
  Serial1.begin(57600);    // Puerto serial para el convertidor USB-TTL de la Raspberry pi3
  // NOTA: 57600 funciona bien ya que es a 16MHz el Arduino MEGA, y el Serial 3 no sirve cuando se utiliza Software Serial
}
// ============================================ FUNCIONES  ==========================================================
// ======================================= DATOS ULTRASÓNICOS Y PUERTO SERIAL ARDUINO UNO ================================================================
void leerCamara(void)
{
  if(Serial1.available()){
    String leerCamara = Serial1.readStringUntil(122);     // 122 es el ASCII de z, el fin de la cadena s(Coordx)a(Coordy)bz, en la rasp no poner letras raras   
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    int indice_0 = leerCamara.lastIndexOf('s');
    int indice_1 = leerCamara.indexOf('a',indice_0+1); //("a",0)
    int indice_2 = leerCamara.indexOf('b',indice_1+1); //("b",indice_1+1)
     
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = leerCamara.substring(indice_0+1,indice_1);
    String cadena_2 = leerCamara.substring(indice_1+1,indice_2);

    // Parse.Int de la cadena
    Coordx_cono = cadena_1.toInt();
    Saturacion = cadena_2.toInt();

    // Despliegue de vflores de los ultrasónicos
    Serial.print("Coordx_cono: ");
    Serial.print(Coordx_cono);
    Serial.print("  Saturacion: ");
    Serial.println(Saturacion);
  }
  }
}

// ===================================================== VOID LOOP ============================================================
void loop()
{
  leerCamara();   
}
