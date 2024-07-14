#define motorA1  17
#define motorA2  12
#define motorB1  18
#define motorB2  19
#define buzzer   25 
#define lights   26

// Setting PWM properties
const int freq = 5000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 255;

char dat;
int i=0;
int j=0;
 int vSpeed=200;

 //Calculate Battery Level
  const float maxBattery = 8.0;// Change value to your max battery voltage level! 
  int perVolt;                 // Percentage variable 
  float voltage = 0.0;         // Read battery voltage
  int level;
// Use it to make a delay... without delay() function!
  long previousMillis = -1000*10;// -1000*10=-10sec. to read the first value. If you use 0 then you will take the first value after 10sec.  
  long interval = 1000*10;       // interval at which to read battery voltage, change it if you want! (10*1000=10sec)
  unsigned long currentMillis;   //unsigned long currentMillis;

void setup() {
  //-------------declare kaki dahulu-----------------//
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(lights, OUTPUT); 
  //-------------------buka ledc punya channel-----//
  ledcSetup(3, 5000, 8); // channel number(0-15), frequensi 5000 hz PWM, 8-bit 
  ledcSetup(4, 5000, 8); // 5000 hz PWM, 8-bit resolution

  //--------setting channet bagi setiap nilai PWM------------//
  ledcAttachPin(motorA1, 3); 
  ledcAttachPin(motorA2, 4); 

  Serial.begin(115200);

}

void loop() {

  if(Serial.available()> 0){
    dat = Serial.read();
    Serial.println(dat);}


  if (dat == '0'){
      vSpeed=0;}
    else if (dat == '1'){
      vSpeed=100;}
    else if (dat == '2'){
      vSpeed=180;}
    else if (dat == '3'){
      vSpeed=200;}
    else if (dat == '4'){
      vSpeed=255;}

  if (dat=='A'){
  ledcWrite(3, vSpeed);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
  ledcWrite(4, 0);
  }

  else if (dat=='S'){
  ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
  ledcWrite(4, 0);
  }

  //If state is equal with letter 'V', play (or stop) horn sound
    else if (dat == 'V'){
      Serial.println(j);
      if (j==0){  
         digitalWrite(buzzer, HIGH);//Speaker on 
          Serial.println(" HON BERBUNYI");
         j=1;
      }
      else if (j==1){
         digitalWrite(buzzer, LOW);
         Serial.println(" HON STOP");
         j=0;
      }
      dat='n';  
    }

        /************************Lights*****************************/
  //If state is equal with letter 'W', turn leds on or of off
    else if (dat == 'W') {
      if (i==0){  
         digitalWrite(lights, HIGH); 
         i=1;
      }
      else if (i==1){
         digitalWrite(lights, LOW); 
         i=0;
      }
      dat='n';
    }


     //Read battery voltage every 10sec.
    currentMillis = millis();
    if(currentMillis - (previousMillis) > (interval)) {
       previousMillis = currentMillis; 
       //Read voltage from analog pin A0 and make calibration:
       voltage = (analogRead(35)*3.3 / 4096.0)*8.2;
       //Calculate percentage...
       perVolt = (voltage*100)/ maxBattery;
       if      (perVolt<=75)               { level=0; }
       else if (perVolt>75 && perVolt<=80) { level=1; }    //        Battery level
       else if (perVolt>80 && perVolt<=85) { level=2; }    //Min ------------------------   Max
       else if (perVolt>85 && perVolt<=90) { level=3; }    //    | 0 | 1 | 2 | 3 | 4 | 5 | >
       else if (perVolt>90 && perVolt<=95) { level=4; }    //    ------------------------
       else if (perVolt>95)                { level=5; }   
       Serial.println(level);    
    }
}

  
