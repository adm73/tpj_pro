#include <dht.h>
#include <Servo.h>
/*************************************/
dht DHT;
/************ COVER    ****************/
Servo servo1;
Servo servo2;
int LdrIN1 = A2; //light sensor 1
int LdrIN2 = A3; //light sensor 2
int lightValue1 = 0;
int lightValue2 = 0;
/************  COVER END  ****************/

/**************  WATER PUMP  *******************/
int enablePin = 11;
int in1Pin = 10;
//int in2Pin = 9;
int pinOut = 13;
//int switchPin = 7;
int potPin = A0;
//int statusPin= 13;
int soilPin = A1;
int soilValue;
int gled = 9;
int yled = 8;
int rled = 7;

/*********   WATER PUMP END  **************/

/*****  TEMPERATURE CONTROL SYSTEM  *******/
int enablePin2 = 4;
int in1Pin2 = 3;
int in2Pin2 = 2;
int switchPin2 = 1;

int potPin2 = 0; //***///**//*CAUTION---------------

//int statusPin= 13;
int DHT11_PIN= A4;
/***  TEMPERATURE CONTROL SYSTEM END ***/

void setup()
{
  Serial.begin(9600);
  pinMode(in1Pin, OUTPUT); //pin 10
  //pinMode(in2Pin, OUTPUT);
  pinMode(pinOut, OUTPUT); //pin 9
  pinMode(enablePin, OUTPUT); //pin 11
  //pinMode(switchPin, INPUT_PULLUP); //pin 7
  //pinMode(statusPin,OUTPUT); //pin 13 // not in use
  pinMode(gled, OUTPUT); //green
  pinMode(yled, OUTPUT); //yellow
  pinMode(rled, OUTPUT); //red
  pinMode(LdrIN1, INPUT);
  pinMode(LdrIN2, INPUT);
  //pinMode(ServoOut, OUTPUT);
   Serial.println("Light Sensor 1,\t\tLight Sensor 2,\t\tSoil Moisture,\t\tCover Status,\t\tTemperature,\t\tHumidity,\t\tFan Status");
   servo1.attach(6);
   servo2.attach(5);
   
/*********  TEMPERATURE CONTROL SYSTEM  **************/
  pinMode(DHT11_PIN,INPUT);
  pinMode(in1Pin2, OUTPUT);/*PIN10*/
  pinMode(in2Pin2, OUTPUT);/*PIN9*/
  pinMode(enablePin2, OUTPUT);/**/
  pinMode(switchPin2, INPUT_PULLUP);
  //pinMode(statusPin,OUTPUT);
/*********  TEMPERATURE CONTROL SYSTEM END  ******************/
}

void loop()
{
  lightValue1 = analogRead(LdrIN1);
  lightValue2 = analogRead(LdrIN2);
  Serial.print("    ");
  Serial.print(lightValue1);
  Serial.print(",      \t\t  ");
  Serial.print(lightValue2);
  Serial.print(", \t\t\t");
  Serial.print(soilValue);
  Serial.print(",  \t\t");
  
  //digitalWrite(13,HIGH);
  int speed = analogRead(potPin) / 4;
  soilValue = analogRead(soilPin);
  //boolean reverse = digitalRead(switchPin);
  boolean reverse = digitalRead(pinOut);
  ///////////////////////////////////////////
  if(lightValue1 < 500 && lightValue2 < 500)
  {
    Serial.print("Cover is OFF");
    servo1.write(180);
    //digitalWrite(ServoOut, HIGH);
  } 
  else
  {
    Serial.print("Cover is ON");
    servo1.write(10);
   // digitalWrite(ServoOut, HIGH);   
  }
  /////////////////////////////////////////
  if(lightValue1 > 500 || lightValue2 > 500)
  {
    servo2.write(180);
    //digitalWrite(ServoOut, HIGH);
  }
  else
  {
    servo2.write(10);
   // digitalWrite(ServoOut, HIGH);   
  }  
  //////////////////////////////////////////
  if(soilValue > 800)
    setMotor(speed, reverse);
  else
    setMotor(1, reverse);

soilValue = analogRead(soilPin);
  if(soilValue < 500)
  {
    digitalWrite(gled, HIGH); //9
    digitalWrite(yled, LOW);//8
    digitalWrite(rled, LOW);//7
  }
  else if(soilValue >= 500 && soilValue < 800)
  {
    digitalWrite(yled, HIGH);
    digitalWrite(rled, LOW);
    digitalWrite(gled, LOW); 
  }
  else
  {
    digitalWrite(rled, HIGH);
    digitalWrite(yled, LOW);
    digitalWrite(gled, LOW);
  }
/********* TEMPERATURE CONTROL SYSTEM  *********/

  int chk = DHT.read11(DHT11_PIN);
  //Serial.print("Temperature = ");
  Serial.print("  \t\t");
  Serial.print(DHT.temperature);
  Serial.print(" C\t");
  //Serial.print("Humidity = ");
  Serial.print("\t\t");
  Serial.print(DHT.humidity);
  Serial.print(" %");

 int speed2 = analogRead(potPin2) / 4;
  boolean reverse2 = digitalRead(switchPin2);
  setMotor2(speed2, reverse2);
  
  if(DHT.temperature >= 30)
  {
    Serial.print("  \t\t");
   // Serial.print(" ");
    Serial.println("FAN IS ON");
    //digitalWrite(in2Pin2,HIGH);
   digitalWrite(enablePin2,HIGH);
    //digitalWrite(enablePin2,HIGH);
  }
  else
  {
    Serial.print("  \t\t");
   // Serial.print(" ");
    Serial.println("FAN IS OFF");
    //digitalWrite(in2Pin2,LOW);
  }
/********* TEMPERATURE CONTROL SYSTEM END  ************/
   delay (1000); 
}

void setMotor(int speed,boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin,  reverse);
  digitalWrite(pinOut, HIGH);
}

 void setMotor2(int speed2, boolean reverse2)
{
  analogWrite(enablePin, speed2);
  digitalWrite(in1Pin2, ! reverse2);
  digitalWrite(in2Pin2, reverse2);
}
