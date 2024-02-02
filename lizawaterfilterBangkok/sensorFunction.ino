void sensorVar(){

  int dataVar = analogRead(AnalogSensor2);
  Serial.print("Sensor Ambient : ");
  Serial.println(dataVar);

  float ambient = dataVar * 0.0976;// percentage calculation
  Serial.print("peratus intensity : ");
  Serial.println(ambient,1);

  lcd.setCursor(0,2);
  lcd.print("cahaya: ");
  lcd.setCursor(8,2);
  lcd.print(dataVar);

  dtostrf(dataVar, 1, 2, varPotString);
  client.publish("kolamiot02/cahaya", varPotString);
}

void sensorsuhu(void)
{ 
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Sensor suhu : ");
  Serial.println(tempC);
  
  lcd.setCursor(0,1);
  lcd.print("suhu: ");
  lcd.setCursor(6,1);
  lcd.print(tempC,1);

  dtostrf(tempC, 1, 2, varPotString);
  client.publish("kolamiot02/suhu", varPotString);
  
}


void sensorPH()
{

  voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
  Serial.print("voltage:");
  Serial.println(voltage, 1);
   
  float phValue = ph.readPH(voltage, 28); // convert voltage to pH with temperature compensation
  Serial.print("pH:");
  Serial.println(phValue, 1);

  lcd.setCursor(0,2);
  lcd.print("pH: ");
  lcd.setCursor(6,2);
  lcd.print(phValue,1);

  dtostrf(phValue, 1, 2, varPotString);
  client.publish("kolamiot02/ph", varPotString);
 
}


void sensorIR(){

  int dataIR = analogRead(sensorIRed);
  Serial.print("paras IR : ");
  Serial.println(dataIR);

  //lcd.setCursor(0,2);
  //lcd.print("cahaya: ");
  //lcd.setCursor(8,2);
  //lcd.print(dataVar);

  dtostrf(dataIR, 1, 2, varPotString);
  client.publish("kolamiot02/parasIR", varPotString);
}