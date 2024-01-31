 #include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <DHT.h>;
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//-----associte pin number with sensor/actuator 
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//----ISTIHAR GPIO PIN ---------------------------------------------------------------------------------

//---=on board------
//#define oneWireBus 4  //--SENSOR SUHU DS18B20
#define Relay01 27   //relay 1
#define Relay02 26   // relay 2
#define buzzer  25     // Buzzer
//--- device/sensor OFF board ------
#define FlowSensor  27
#define ldrsensor  34

#define SENSOR_PIN  15 // ESP32 pin GIOP21 connected to DS18B20 sensor's DQ pin
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//----------------------------------- end -------------------------------------------------------------


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//----ISTIHAR SENSOR SUHU DHT ---------------------------------------------------------------------
 
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
//----------------------------------- end -------------------------------------------------------------


//---- PENETAPAN WIFI ---------------------------------------------------------------------------------
//------WIFI PTSB---
/*const char* ssid = "PTSB STAFF";  
const char* password = ""; */

//------WIFI AZA---
const char* ssid = "Alias FredoX_2.4GHz@unifi";  
const char* password = "0135335045";


WiFiClient espClient;                                                                          
PubSubClient client(espClient); //MQTT
const char* mqtt_server = "broker.hivemq.com";

//----------------------------------- end -------------------------------------------------------------

//---- PEMBOLEHUBAH DATA---------------------------------------------------------------------------------
float sensorAnalog1;
float sensorAnalog2;
float temperatureC;
char varPotString[8];
float dataMoisture;
long lastMsg = 0;
char msg[50];
int value = 0;
//----------------------------------- end -------------------------------------------------------------



void setup() {

  pinMode (Relay01,OUTPUT);
  pinMode (Relay02,OUTPUT);
  pinMode (buzzer,OUTPUT);

Serial.begin(115200);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000);
  display.clearDisplay();
  
   dht.begin();
    
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(100);  

 display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
   display.setCursor(0,0);
  display.println("IoT Socket Control");
//  display.setCursor(40, 20);
  display.display();
  delay(500);
    
}

void loop() {
if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 10000) {  
    lastMsg = now;

//-------------------------------------------------------------------------------
//                     -------------------suhu --------------------------
     
     //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temperatureC= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temperatureC);
    Serial.println(" Celsius");
    //delay(2000); //Delay 2 sec.
    
    
   
    if ((temperatureC >23) && (temperatureC < 28) ){
       Serial.println("Suhu Normal");
       client.publish("socketaza/suhu/status", "Suhu Normal");
       digitalWrite(buzzer,LOW);
    }

    else if ((temperatureC >29) && (temperatureC < 38) ){
       Serial.println("Suhu Naik");
       client.publish("socketaza/suhu/status", "Amaran Suhu NaiK");
       digitalWrite(buzzer,LOW);
    }

    else if (temperatureC >38){
       Serial.println("Suhu PANAS");
       client.publish("socketaza/suhu/status", "Suhu PANAS");
     //  bot.sendMessage(CHAT_ID, "SUHU Panas..!", "");
       digitalWrite(buzzer,HIGH);
       delay(100);
    }
    else if (temperatureC <23) {
       Serial.print("Suhu MENURUN");
       client.publish("socketaza/suhu/status", "Suhu MENURUN");
    //   bot.sendMessage(CHAT_ID, "SUHU menurun..!", "");
       digitalWrite(buzzer,LOW);
    }
    else
    {
      digitalWrite(buzzer,LOW);
    }
        
    varPotString[8];
    dtostrf(temperatureC, 1, 1, varPotString);
    Serial.print("Analog suhu: ");
    Serial.println(varPotString);
    client.publish("socketaza/suhu", varPotString);

    dtostrf(hum, 1, 1, varPotString);
    Serial.println(varPotString);
    client.publish("socketaza/hum", varPotString);
    
//---------------------------------------------------end --------------------------
//-------------------------------------------------------------------------------

 

//----------- Sensor Moisture ----------------------------------

int dataLDR = analogRead(ldrsensor);  
    Serial.print("LDR Value : ");
    Serial.println(dataLDR);

  

    // Convert the value to a char array
    char varPotString[8];
    dtostrf(dataLDR, 1, 2, varPotString);
    Serial.print("Analog LDR sensor: ");
    Serial.println(varPotString);
    client.publish("socketaza/ldr", varPotString);

//----------- Sensor Data Readibg here ----------------------------------

   // Put your coding here
   
//-------------------end ------------------------------------------------
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("IoT Socket Control");
    display.setTextSize(1);
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0,20);
    display.print("SUHU:");
    display.print(temperatureC,1);

     //  display.setTextColor(WHITE,BLACK);
    display.setCursor(0,30);
    display.print("HUM :");
    display.print(hum);
    display.display();
  //  display.setTextColor(WHITE,BLACK);
    display.setCursor(0,40);
    display.print("LDR :");
    display.print(dataLDR);
    display.display();

 }
}

 
