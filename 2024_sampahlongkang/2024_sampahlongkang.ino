 #include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>


//-----associte pin number with sensor/actuator 

//----ISTIHAR GPIO PIN ---------------------------------------------------------------------------------

//---=on board------
#define SENSOR_PIN  15 // ESP32 pin GIOP21 connected to DS18B20 sensor's DQ pin
#define Relay01 27   //relay 1
#define Relay02 26   // relay 2
#define buzzer  2     // Buzzer
//--- device/sensor OFF board ------
#define distanceSensor  36
#define tiltSensor  4

//----------------------------------- end -------------------------------------------------------------

//----ISTIHAR SENSOR SUHU DS18B20 ---------------------------------------------------------------------
 
OneWire oneWire(SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);
//----------------------------------- end -------------------------------------------------------------


//---- PENETAPAN WIFI ---------------------------------------------------------------------------------
//------WIFI PTSB---
/*const char* ssid = "PTSB STAFF";  
const char* password = ""; */

//------WIFI AZA---
//const char* ssid = "Alias FredoX_2.4GHz@unifi";  
//const char* password = "0135335045";

const char* ssid = "GalaxyA";  
const char* password = "1234509876";

//-----WiFi KKSBJ --------------------------------
//const char* ssid = "GUEST";  
//const char* password = ""; 

WiFiClient espClient;                                                                          
PubSubClient client(espClient); //MQTT
const char* mqtt_server = "broker.hivemq.com";

//----------------------------------- end -------------------------------------------------------------

//---- PEMBOLEHUBAH DATA---------------------------------------------------------------------------------
int dataDistanceSensor;
float tempC; // temperature in Celsius
char varPotString[8];
long lastMsg = 0;
char msg[50];
int value = 0;

int tiltState = 0;
//----------------------------------- end -------------------------------------------------------------



void setup() {

  pinMode(Relay01,OUTPUT);
  pinMode(Relay02,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(tiltSensor, INPUT);

  Serial.begin(115200);

  DS18B20.begin();
    
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(100);  
  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 1000) {  
    lastMsg = now;

//-------------------------------------------------------------------------------
//                     -------------------suhu --------------------------
  Serial.println("-----------------URBAN FARMING ------------------");  
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  Serial.print("Temperature: ");
  Serial.print(tempC);    // print the temperature in °C
  Serial.println("°C");
   
  dtostrf(tempC, 1, 1, varPotString);
  client.publish("trash/suhu", varPotString);
    
//---------------------------------------------------end --------------------------
//----------- Sensor Food Level ----------------------------------
Serial.println("-----------------Makanan Ikan ------------------");
  float volts = analogRead(distanceSensor) * 0.0008056640625; // value from sensor * (3.3/4096)
  int distance = 29.988 * pow( volts, -1.173);


  if (distance <= 80) {
    Serial.println(distance);
    dtostrf(distance, 1, 1, varPotString);
    client.publish("trash/foodtank", varPotString);
  }
 //-----------end ----------------------------------

//----------- Sensor Var ----------------------------------

  tiltState = digitalRead(tiltSensor);
 Serial.println(tiltState);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (tiltState == HIGH) {
    // turn LED on:
    digitalWrite(buzzer, HIGH);
    
   client.publish("trash/tanklid", "Food Lid is OPEN");
  } else {
    // turn LED off:
    digitalWrite(buzzer, LOW);
    client.publish("trash/tanklid", "Food Lid is CLOSE");
  }


}


}

 
