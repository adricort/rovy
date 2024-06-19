float leer_US_IMU(int caso)
{
  float IMU_anterior;
  
  if(Serial2.available()>0){
    String US_IMU = Serial2.readStringUntil(36);     // 36 es el ASCII de $, el fin de la cadena s(NUMERO0)a(NUMERO1)b(NUMERO2)c(NUMERO3)d(NUMERO4)e(NUMERO5)f(NUMERO)6g(NUMERO7)h$
    
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    int indice_0 = US_IMU.lastIndexOf('s');
    int indice_1 = US_IMU.indexOf('a',indice_0+1); //("a",0)
    int indice_2 = US_IMU.indexOf('b',indice_1+1); //("b",indice_1+1)
    int indice_3 = US_IMU.indexOf('c',indice_2+1); //("c",indice_2+1)
    int indice_4 = US_IMU.indexOf('d',indice_3+1); //("d",indice_3+1)
    int indice_5 = US_IMU.indexOf('e',indice_4+1); //("e",indice_4+1)
    int indice_6 = US_IMU.indexOf('f',indice_5+1); //("f",indice_5+1)
    
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = US_IMU.substring(indice_0+1,indice_1);
    String cadena_2 = US_IMU.substring(indice_1+1,indice_2);
    String cadena_3 = US_IMU.substring(indice_2+1,indice_3);
    String cadena_4 = US_IMU.substring(indice_3+1,indice_4);
    String cadena_5 = US_IMU.substring(indice_4+1,indice_5);
    String cadena_6 = US_IMU.substring(indice_5+1,indice_6);

    // Parse.Int o Float de la cadena
    int uS_1 = cadena_1.toInt();
    int uS_2 = cadena_2.toInt();
    int uS_3 = cadena_3.toInt();
    int uS_4 = cadena_4.toInt();
    int uS_5 = cadena_5.toInt();
    double IMU = cadena_6.toFloat();   // 360 - el ángulo porque crece al revés
    
    if(!primer_dato_IMU)
    {
      IMU_anterior = IMU; // Actualiza el valor
      primer_dato_IMU = true;
    }

    // El siguiente condicional sirve para filtrar el ruido que genera tener combinados los US (int) y la IMU (float)
    if( uS_1 <= 1 && uS_3 <= 1 && uS_5 <= 1) 
    { 
      IMU_anterior = IMU; // Actualiza el valor
       
       // Despliegue de vflores de los ultrasónicos
      /*Serial.print("US1: ");
      Serial.print(uS_1);
      Serial.print(" US2: ");
      Serial.print(uS_2);
      Serial.print(" US3: ");
      Serial.print(uS_3);
      Serial.print(" US4: ");
      Serial.print(uS_4);
      Serial.print(" US5: ");
      Serial.print(uS_5);
      Serial.print(" IMU: ");
      Serial.println(IMU);*/
    }

    switch(caso)
    {
      case 0:   // Despliega el valor del error de los ultrasónicos
        eu = uS_1*(-1) + uS_5*(1);
        return eu;
        break;
      case 1:   // Despliega el valor de la IMU
        return IMU_anterior;
        break;
      case 2:
        return uS_3;
      default:
        break;
    }

  }
}
