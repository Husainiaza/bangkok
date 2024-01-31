#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h> 
#include <LiquidCrystal_I2C.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//---onboard ---------------
#define SENSOR_PIN  15 // ESP32 pin GIOP21 connected to DS18B20 sensor's DQ pin
#define relay1 26
#define relay2 27
#define buzzer 25
//---offboard--------------
#define pinsuis1 39 
#define pinsuis2 33 
#define pinsenVR 36
#define pinsenLDR 34
#define servoPin 13
//---ultrasonic  pin connection---
#define trigPin  2
#define echoPin  4


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OneWire oneWire(SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);
Servo myservo; 

//----ISTIHAR LCD I2C --------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27,20,4);

float tempC; // temperature in Celsius
float tempF; // temperature in Fahrenheit

int dataSuis1;
int dataSuis2;
int datasenVR;
int datasenLDR;
int distance;
int pos = 0; 

void setup() {
pinMode(pinsuis1,INPUT);
pinMode(pinsuis2,INPUT);
pinMode(relay1,OUTPUT);
pinMode(relay2,OUTPUT);
pinMode(buzzer,OUTPUT);

pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT);



// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"
  Serial.begin(115200); // initialize serial
  DS18B20.begin();    // initialize the DS18B20 sensor

  //myservo.attach(servoPin); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("ESP32 Project Board");
  lcd.setCursor(0,1);
  lcd.print("   KOLEJ KOMUNITI   ");
  lcd.setCursor(0,2);
  lcd.print("SEBERANG JAYA (KKSJ)");
  lcd.setCursor(0,3);
  lcd.print("  KIK HB 2023 PTSS  ");
  
}

void loop() {
   
  DS18B20.requestTemperatures();       // send the command to get temperatures
  tempC = DS18B20.getTempCByIndex(0);  // read temperature in °C
  tempF = tempC * 9 / 5 + 32; // convert °C to °F

  dataSuis1= digitalRead(pinsuis1);
  Serial.print(" dataSuis1 :");
  Serial.println(dataSuis1);

  dataSuis2= digitalRead(pinsuis2);
  Serial.print(" dataSuis2 :");
  Serial.println(dataSuis2);

  datasenVR = analogRead(pinsenVR);
  Serial.print("data Var    :");
  Serial.println(datasenVR);

  datasenLDR = analogRead(pinsenLDR);
  Serial.print("data Cahaya :");
  Serial.println(datasenLDR);
   
  Serial.print("Temperature: ");
  Serial.print(tempC);    // print the temperature in °C
  Serial.print("°C");
  Serial.print("  ~  ");  // separator between °C and °F
  Serial.print(tempF);    // print the temperature in °F
  Serial.println("°F");

  if (dataSuis1 ==1)
  {
    digitalWrite(relay1,HIGH);
    digitalWrite(relay2,LOW);
    digitalWrite(buzzer,HIGH);
    delay(500);
    digitalWrite(relay1,LOW);
    digitalWrite(relay2,HIGH);
    digitalWrite(buzzer,LOW);
    delay(500);
  }

  else if (dataSuis2 ==1)
  {
    servoaza();
  }
  else
  {
    digitalWrite(buzzer,LOW);
    digitalWrite(relay1,HIGH);
    digitalWrite(relay2,HIGH);
  }


ultrasonic();


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("IoT Project Board");
  //display.display(); 
 
  //display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("SUHU  :");
  display.setCursor(50, 20);
  display.print(tempC,1);
 
  display.setCursor(0, 30);
  display.println("SUIS 1: ");
  display.setCursor(40, 30);
  display.println(dataSuis1);

  display.setCursor(60, 30);
  display.println("D: ");
  display.setCursor(80, 30);
  display.println(distance);

  display.setCursor(0, 40);
  display.println("analog Var : ");
  display.setCursor(80, 40);
  display.println(datasenVR);

  display.setCursor(0, 50);
  display.println("analog LDR : ");
  display.setCursor(80, 50);
  display.println(datasenLDR);
  
  display.display();
  delay(500);
}
