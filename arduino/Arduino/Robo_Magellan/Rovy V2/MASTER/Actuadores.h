// ---------------------------------- FUNCIONES------------------------------------------------
void mot(int vel_mizq, int vel_mder)  // Motores adelante con velocidad
{
  // Motores izquierdos
  if(vel_mizq >= 0)
  {
    roboclaws.ForwardM1(address0,vel_mizq); //start Motor1 forward at half speed
    roboclaws.ForwardM1(address1,vel_mizq); //start Motor1 forward at half speed
    roboclaws.ForwardM1(address2,vel_mizq); //start Motor1 forward at half speed
  }
  else if(vel_mizq < 0)
  {
    vel_mizq = vel_mizq*(-1);
    roboclaws.BackwardM1(address0,vel_mizq); //start Motor1 forward at half speed
    roboclaws.BackwardM1(address1,vel_mizq); //start Motor1 forward at half speed
    roboclaws.BackwardM1(address2,vel_mizq); //start Motor1 forward at half speed
  }

  // Motores derechos
  if(vel_mder >= 0)
  {
    roboclaws.ForwardM2(address0,vel_mder); //start Motor1 forward at half speed
    roboclaws.ForwardM2(address1,vel_mder); //start Motor1 forward at half speed
    roboclaws.ForwardM2(address2,vel_mder); //start Motor1 forward at half speed
  }
  else if(vel_mder < 0)
  {
    vel_mder = vel_mder*(-1);
    roboclaws.BackwardM2(address0,vel_mder); //start Motor1 forward at half speed
    roboclaws.BackwardM2(address1,vel_mder); //start Motor1 forward at half speed
    roboclaws.BackwardM2(address2,vel_mder); //start Motor1 forward at half speed
  }
}

void attachServos(void)
{
  Camara_neck.attach(pin_neck);
  Camara_head.attach(pin_head);
  Bandera_izq.attach(pin_band_izq);
  Bandera_der.attach(pin_band_der);
}

void detachServos(void)
{
  Camara_neck.detach();
  Camara_head.detach();
  Bandera_izq.detach();
  Bandera_der.detach();
}

void servos(int ang_band_izq, int ang_band_der, int ang_neck, int ang_head, int tiempo)     // Escritura de servos
{
  attachServos();
  Bandera_izq.write(ang_band_izq);
  Bandera_der.write(ang_band_der);
  Camara_neck.write(ang_neck);
  Camara_head.write(ang_head);
  delay(tiempo);
  detachServos();
}

void originServos()
{
  attachServos();
  Bandera_izq.write(min_band_izq);
  Bandera_der.write(min_band_der);
  Camara_neck.write(cent_neck);
  Camara_head.write(cent_head);
  delay(700);
  detachServos();
}
