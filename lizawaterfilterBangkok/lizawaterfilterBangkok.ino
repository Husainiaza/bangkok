#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DFRobot_ESP_PH.h"

//----ISTIHAR GPIO PIN anda kumpulkan disini ---------------------------------------------------------------------------------


//---=on board------
#define oneWireBus 15  //--SENSOR SUHU DS18B20
#define Relay01 27   //relay 1
#define Relay02 26   // relay 2
#define buzzer  2     // Buzzer
//--- device/sensor OFF board ------
#define sensorIRed  33
#define AnalogSensor2  35
#define temtLightSensor 39


#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3300 //the esp voltage supply value
#define PH_PIN 36   //the esp gpio data pin number
float voltage, phValue, temperature=25;


DFRobot_ESP_PH ph;
//----------------------------------- end -------------------------------------------------------------


//----ISTIHAR LCD I2C --------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27,20,4);




//----ISTIHAR SENSOR SUHU DS18B20 ---------------------------------------------------------------------
 
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
//----------------------------------- end -------------------------------------------------------------





//---- PENETAPAN WIFI ---------------------------------------------------------------------------------


//const char* ssid = "xxxxxxxxxxxxxxx nama wifi SSID anda";  
//const char* password = "xxxxxxxxxxx password wifi SSID anda";

//const char* ssid = "Alias FredoX_2.4GHz@unifi";  
//const char* password = "0135335045";

const char* ssid = "GalaxyA3";  
const char* password = "1234509876"; 


//-------------------------------------------------------------------------------
WiFiClient espClient;                                                                          
PubSubClient client(espClient); //MQTT
const char* mqtt_server = "broker.hivemq.com";


//----------------------------------- end -------------------------------------------------------------


//---- PEMBOLEHUBAH DATA---------------------------------------------------------------------------------


char varPotString[8];
long lastMsg = 0;
char msg[50];
int value = 0;
float light;


//----------------------------------- end ---------------------------------------------


void setup() {
  pinMode (Relay01,OUTPUT);
  pinMode (Relay02,OUTPUT);
  pinMode (buzzer,OUTPUT);

  delay(100);
  digitalWrite(Relay01,LOW);
  digitalWrite(Relay02,LOW);
  digitalWrite(buzzer,LOW);
    
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(100);  
  
  Serial.begin(115200);
  sensors.begin();
  ph.begin();
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("System is starting..");
  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);
  delay(500);

  lcd.setCursor(0,1);
  lcd.print("ESP32 PROJECT BOARD ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   KOLAM IoT 2023   ");
}


void loop() {
if (!client.connected()) {
    reconnect();
  }
  client.loop();


  long now = millis();


  if (now - lastMsg > 5000) {  
    lastMsg = now;


          //-----------------------------------------------------------------------
          //---mulakan sensor kod anda disini--
    sensorsuhu();
    sensorPH();
    sensorIR();
          //-------------------end ------------------------------------------------
  }
}
