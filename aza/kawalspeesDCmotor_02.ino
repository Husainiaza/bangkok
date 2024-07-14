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

  if (dat=='A'){
  ledcWrite(3, 255);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
  ledcWrite(4, 0);
  }

 else if (dat=='B'){
  ledcWrite(3, 180);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
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
}

 
