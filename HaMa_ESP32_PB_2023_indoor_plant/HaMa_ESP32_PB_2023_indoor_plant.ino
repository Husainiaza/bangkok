#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>


//----ISTIHAR GPIO PIN anda kumpulkan disini ---------------------------------------------------------------------------------

//---=on board------
#define oneWireBus 15  //--SENSOR SUHU DS18B20
#define Relay01 27   //relay 1
#define Relay02 26   // relay 2
#define buzzer  25     // Buzzer
//--- device/sensor OFF board ------
#define sensorSoil  33
#define sensorIRed  35
#define pinsensorCahaya  36
#define temtLightSensor 39

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//----------------------------------- end -------------------------------------------------------------

//----ISTIHAR SENSOR SUHU DS18B20 ---------------------------------------------------------------------
 
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
//----------------------------------- end -------------------------------------------------------------


//---- PENETAPAN WIFI ---------------------------------------------------------------------------------


//const char* ssid = "xxxxxxxxxxxxxxx nama wifi SSID anda";  
//const char* password = "xxxxxxxxxxx password wifi SSID anda";

//const char* ssid = "Alias FredoX_2.4GHz@unifi";  
//const char* password = "0135335045";

const char* ssid = "dlink-Hama";  
const char* password = "12345";

//-------------------------------------------------------------------------------
WiFiClient espClient;                                                                          
PubSubClient client(espClient); //MQTT
//const char* mqtt_server = "broker.hivemq.com";

const char* mqtt_server = "192.168.0.102";

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

  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }   
  
  delay(2000);
  display.clearDisplay();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(100);  
  
  Serial.begin(115200);
  sensors.begin();
  systemMula();
  display.clearDisplay();
  display.display();
}


void loop() {
if (!client.connected()) {
    reconnect();
  }
  client.loop();


  long now = millis();


  if (now - lastMsg > 3000) {  
    lastMsg = now;


          //-----------------------------------------------------------------------
          //---mulakan sensor kod anda disini--
    sensorsuhu();
    sensorCahaya();
    sensorTanah();

    
    display.setCursor(0,0);
    display.println("IoT INDOOR PLANT");
    //display.display();
          //-------------------end ------------------------------------------------
  }
}
