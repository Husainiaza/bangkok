#define motorA1  17
#define motorA2  12
#define motorB1  18
#define motorB2  19

// Setting PWM properties
const int freq = 5000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 255;


char dat;

void setup() {
  //-------------declare kaki dahulu-----------------//
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
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

if (dat=='B'){
  ledcWrite(3, 180);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
  ledcWrite(4, 0);
  }

  if (dat=='S'){
  ledcWrite(3, 0);//ledcWrite untuk jalankan pwm (nilai 3 tu adalah nomber channel
  ledcWrite(4, 0);
  }
}

  
