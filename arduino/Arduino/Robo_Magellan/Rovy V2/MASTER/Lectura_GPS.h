void leerGPS(void)
{
  if(Serial.available()>0){
    String GPS = Serial.readStringUntil(36);     // 36 es el ASCII de $, el fin de la cadena s(NUMERO0)a(NUMERO1)b(NUMERO2)c(NUMERO3)d(NUMERO4)e(NUMERO5)f(NUMERO)6g(NUMERO7)h$
    
    // El primer parámetro es el límite del índice y el segundo es el inicio del índice
    float indice_0 = GPS.lastIndexOf('s');
    float indice_1 = GPS.indexOf('a',indice_0+1); //("a",0)
    float indice_2 = GPS.indexOf('b',indice_1+1); //("b",indice_1+1)
    float indice_3 = GPS.indexOf('c',indice_2+1); //("c",indice_2+1)
    float indice_4 = GPS.indexOf('d',indice_3+1); //("d",indice_3+1)
    // Se guarda la cadena principal en subcadenas en las variables a usar
    String cadena_1 = GPS.substring(indice_0+1,indice_1);
    String cadena_2 = GPS.substring(indice_1+1,indice_2);
    String cadena_3 = GPS.substring(indice_2+1,indice_3);
    String cadena_4 = GPS.substring(indice_3+1,indice_4);
    
    lat = cadena_1.toFloat();
    lon = cadena_2.toFloat();
    
    // Truncamos la latitud y longitud para1918.337 sacar la parte decimal
    int lat_int = (int)lat;
    int lon_int = (int)lon;

    // Sacamos la parte decimal
    float lat_decimal_NMEA = lat - lat_int;
    float lon_decimal_NMEA = lon - lon_int;

    // La multiplicamos por 60 para tener formato WGS84
    float lat_decimal_WGS84 = lat_decimal_NMEA*0.6;
    float lon_decimal_WGS84 = lon_decimal_NMEA*0.6;

    // Sacamos la verdadera latitud y longitud y *1000 para no operar con decimales
    lat = (lat -lat_decimal_NMEA)*1000 + lat_decimal_WGS84*1000;
    lon = (lon -lon_decimal_NMEA)*1000 + lon_decimal_WGS84*1000;
    
    // Parse.Int de la cadena
     fix = cadena_3.toFloat();
     satel = cadena_4.toFloat();

     Serial.print("lat: ");Serial.print(lat,3);
     Serial.print(" long: ");Serial.print(lon,3);
     Serial.print(" fix: ");Serial.print(fix);
     Serial.print(" satel: ");Serial.println(satel);
  }
}
