void leerEncoders()
{
  uint8_t status_enc[12]={0,0,0,0,0,0,0,0,0,0,0,0};

  // RoboClaw1 (adelante)
  enc[0] = roboclaws.ReadEncM1(address0, &status_enc[0], &valid_enc[0]);
  enc[1] = roboclaws.ReadEncM2(address0, &status_enc[1], &valid_enc[1]);
  speeds[0] = roboclaws.ReadSpeedM1(address0, &status_enc[2], &valid_enc[2]);
  speeds[1] = roboclaws.ReadSpeedM2(address0, &status_enc[3], &valid_enc[3]);

  // RoboClaw2 (centro)
  enc[2] = roboclaws.ReadEncM1(address1, &status_enc[4], &valid_enc[4]);
  enc[3] = roboclaws.ReadEncM2(address1, &status_enc[5], &valid_enc[5]);
  speeds[2] = roboclaws.ReadSpeedM1(address1, &status_enc[6], &valid_enc[6]);
  speeds[3] = roboclaws.ReadSpeedM2(address1, &status_enc[7], &valid_enc[7]);

  // RoboClaw3 (atrás)
  enc[4] = roboclaws.ReadEncM1(address2, &status_enc[8], &valid_enc[8]);
  enc[5] = roboclaws.ReadEncM2(address2, &status_enc[9], &valid_enc[9]);
  speeds[4] = roboclaws.ReadSpeedM1(address2, &status_enc[10], &valid_enc[10]);
  speeds[5] = roboclaws.ReadSpeedM2(address2, &status_enc[11], &valid_enc[11]);

  for(int j = 0;j < 6; j++)
  {
    //Serial.print("[m]Enc");Serial.print(j);Serial.print(" ");
    if(valid_enc[j] && enc[j] != 0)
    {  
      Serial.print("Encoder #");Serial.print(j+1);Serial.print(": ");Serial.print(enc[j]);Serial.print(" Status: ");Serial.println(valid_enc[j]);
    }
    else
    {
      enc[j] = 0;
      Serial.print("Encoder #");Serial.print(j+1);Serial.print(": ");Serial.print("FAIL");Serial.print(" Status: ");Serial.println(valid_enc[j]);
    }
  }
  
  Serial.println();
}
