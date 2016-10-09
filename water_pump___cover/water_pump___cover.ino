#include <Servo.h>
Servo servo1;
Servo servo2;
/////////////////////////////////
int LdrIN1 = 2; //light sensor 1
int LdrIN2 = 3; //light sensor 2
int lightValue1 = 0;
int lightValue2 = 0;
//////////////////////////////////////
int enablePin = 11;
int in1Pin = 10;
//int in2Pin = 9;
int pinOut = 9;
//int switchPin = 7;
int potPin = 0;
int statusPin= 13;
int soilPin = 1;
int soilValue;
//*************************************************//

void setup()
{
  Serial.begin(9600);
  pinMode(in1Pin, OUTPUT); //pin 10
  //pinMode(in2Pin, OUTPUT);
  pinMode(pinOut, OUTPUT); //pin 9
  pinMode(enablePin, OUTPUT); //pin 11
  //pinMode(switchPin, INPUT_PULLUP); //pin 7
  pinMode(statusPin,OUTPUT); //pin 13 // not in use

  pinMode(8, OUTPUT); //green
  pinMode(7, OUTPUT); //yellow
  pinMode(6, OUTPUT); //red
  
  pinMode(LdrIN1, INPUT);
  pinMode(LdrIN2, INPUT);
  //pinMode(ServoOut, OUTPUT);
   Serial.println("Light Sensor 1,\t\tLight Sensor 2,\t\tSoil Moisture Sensor,\t\tCover Status");
   
   servo1.attach(5);
   servo2.attach(4);
}

void loop()
{
  lightValue1 = analogRead(LdrIN1);
  lightValue2 = analogRead(LdrIN2);
  
  Serial.print("    ");
  Serial.print(lightValue1);
  Serial.print(",      \t\t  ");
  Serial.print(lightValue2);
  Serial.print(",\t\t\t\t");
  Serial.print(soilValue);
  Serial.print("      \t\t\t");
  
  digitalWrite(13,HIGH);
  int speed = analogRead(potPin) / 4;

  soilValue = analogRead(soilPin);
  //boolean reverse = digitalRead(switchPin);
  boolean reverse = digitalRead(pinOut);
  ///////////////////////////////////////////
  
  if(lightValue1 < 500 && lightValue2 < 500)
  {
    Serial.println("ON");
    servo1.write(180);
    //digitalWrite(ServoOut, HIGH);
  }
  
  else
  {
    Serial.println("OFF");
    servo1.write(10);
   // digitalWrite(ServoOut, HIGH);   
  }
  /////////////////////////////////////////
  if(lightValue1 > 500 && lightValue2 > 500)
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
    digitalWrite(8, HIGH); //9
    digitalWrite(7, LOW);//8
    digitalWrite(6, LOW);//7
  }
  else if(soilValue >= 500 && soilValue < 800)
  {
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(8, LOW); 
  }
  else
  {
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
  }
/****/
   delay (1000); 
}

void setMotor(int speed,boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin,  reverse);
  digitalWrite(pinOut, HIGH);
}
