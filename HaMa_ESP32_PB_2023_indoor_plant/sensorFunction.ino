void systemMula(){
  
  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);
  delay(500);
 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.setCursor(0,0);
  delay(500);
  display.setCursor(0,10);
  display.print("ESP32 PROJECT BOARD ");
  display.setCursor(0,20);
  display.print("  INDOOR PLANT  ");
  display.display();
  delay(1000);
}


void sensorsuhu(void) { 
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tempC = sensors.getTempCByIndex(0);
  Serial.print("Sensor suhu : ");
  Serial.println(tempC);
  
  display.setCursor(0,10);
  display.print("SUHU: ");
  display.print(tempC,1);
  display.setCursor(80,10);
  display.print("C");
  display.display();

  dtostrf(tempC, 1, 2, varPotString);
  client.publish("azaiot/suhu", varPotString);
  
}

void sensorTanah(){

  int dataSoil = analogRead(sensorSoil);
  Serial.print("Bacaan Kelembapan Tanah : ");
  Serial.println(dataSoil);

  display.setCursor(0,20);
  display.print("tanah : ");
  display.print(dataSoil);
  display.display();
  
  Serial.print("Soil Moisture Sensor Voltage: ");
  Serial.print(float(dataSoil/4095.0)*3.3); // read sensor
  Serial.println(" V");
 
  dtostrf(dataSoil, 6, 2, varPotString);
  client.publish("azaiot/tanah",varPotString);

 if (dataSoil < 300) {
    Serial.println(" tanah berair ");
    Serial.println(" tanah lembab ");
    client.publish("status/azaiot/tanah", "SOIL  WET");
    //lcd.setCursor(11,1);
    //lcd.print("WET ");
    }
 else if ((dataSoil > 300) && (dataSoil < 700)){
    Serial.println(" tanah lembab ");
    client.publish("status/azaiot/tanah", "SOIL GOOD");
    //lcd.setCursor(11,1);
    //lcd.print("GOOD");
    }
 else if (dataSoil > 701){
    Serial.println(" tanah KERING ");
    client.publish("status/azaiot/tanah", "SOIL DRY");
    // lcd.setCursor(11,1);
    //lcd.print("DRY ");
    }
   else{
    Serial.println(" tanah NORMAL ");
    client.publish("status/azaiot/tanah", "tanah NORMAL");}

  }


void sensorCahaya() {
  
  int dataSensorCahaya = analogRead(pinsensorCahaya);       // read analog input pin 0
  Serial.println(dataSensorCahaya, DEC);  // prints the value read
  delay(100);                        // wait 100ms for next reading

  dtostrf(dataSensorCahaya, 1, 2, varPotString);
  client.publish("azaiot/cahaya",varPotString );
}

void sensorIR(){

  int dataIR = analogRead(sensorIRed);
  Serial.print("paras IR : ");
  Serial.println(dataIR);

  display.setCursor(0,50);
  display.print("Cahaya: ");
  display.print(dataIR,1);
  display.setCursor(80,50);
  display.print("Lux");
  display.display();

  dtostrf(dataIR, 1, 2, varPotString);
  client.publish("azaiot/parasIR",varPotString );
}