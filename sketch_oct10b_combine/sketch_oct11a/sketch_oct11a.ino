#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <dht.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
 
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
int enablePin = 12;
int in1Pin = 11;
//int in2Pin = 9;
int pinOut = 13;
//int switchPin = 7;
int potPin = A0;
//int statusPin= 13;
int soilPin = A4;
int soilValue;
int gled = 10;
int yled = 9;
int rled = 8;
int speed;
int speed2;
/*********   WATER PUMP END  **************/

/*****  TEMPERATURE CONTROL SYSTEM  *******/
int enablePin2 = 5;
int in1Pin2 = 4;
int in2Pin2 = 3;
int switchPin2 =22;

int potPin2 = 24; //***///**//*CAUTION---------------

//int statusPin= 13;
int DHT11_PIN= A5;
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
   Serial.println("-----------------------------------------------------------------------------------------------------------------------------------------------------------");
   servo1.attach(7);
   servo2.attach(6);
   
/*********  TEMPERATURE CONTROL SYSTEM  **************/
  pinMode(DHT11_PIN,INPUT);
  pinMode(in1Pin2, OUTPUT);/*PIN10*/
  pinMode(in2Pin2, OUTPUT);/*PIN9*/
  pinMode(enablePin2, OUTPUT);/**/
  pinMode(switchPin2, INPUT_PULLUP);
  //pinMode(statusPin,OUTPUT);
/*********  TEMPERATURE CONTROL SYSTEM END  ******************/

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.print("Hello, World...");

   delay (2000); 
}

#define MAXVALUE 1024

int val_hum = 879;
int val_brt = 667;
int val_temp = 37;

void loop()
{
    static int is_showed = 0;
  
  if(!is_showed)
  {
    lcd.clear();
    
    delay(2000);
    lcd.setCursor(0, 0); // col row 0 0 
    lcd.print(" HUM    BRT    TEMP ");
    
    // Humidity
    //val_hum = (879 * 100) / 1024;
    lcd.setCursor(1, 2); // 0 1
    lcd.print(val_hum);
    //lcd.print("%");
    
    lcd.print("     ");
    
    // Brightness
    //val_brt = (667 * 100) / 1024;
    lcd.setCursor(8, 2); // row 2 col 8
    lcd.print(val_brt);
    //lcd.print("%");
    
    lcd.print("     ");
        
    // Temperature
    lcd.setCursor(16, 2); // row 2 col 16
    lcd.print(val_temp);
    lcd.write(0xDF);
    lcd.print("C");
    
    is_showed = 1;
  }
  
  lightValue1 = analogRead(LdrIN1);
  lightValue2 = analogRead(LdrIN2);
  Serial.print("    ");
  Serial.print(lightValue1);
  Serial.print(",      \t\t  ");
  Serial.print(lightValue2);
  Serial.print(", \t\t\t   ");
  Serial.print(soilValue);
  Serial.print(",\t\t\t");
  
    // Brightness
    //val_brt = (667 * 100) / 1024;
    lcd.setCursor(8, 2); // row 2 col 8
    lcd.print(lightValue1);
    lcd.print("  ");

  //digitalWrite(13,HIGH);
  int speed = analogRead(potPin) / 4;
  soilValue = analogRead(soilPin);
  
  // Humidity
  lcd.setCursor(1, 2); // 0 1
  lcd.print(soilValue);
  lcd.print("  ");
  
  //boolean reverse = digitalRead(switchPin);
  boolean reverse = digitalRead(pinOut);
  ///////////////////////////////////////////
  if((lightValue1 < 500) && (lightValue2 < 500))
  {
    Serial.print("Cover is OFF");
    servo1.write(180);
    servo2.write(180);
  } 
  else if((lightValue1 > 500) && (lightValue2 > 500))
  {
    Serial.print("Cover is ON");
    servo1.write(10);
    servo2.write(10);
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
  
  // temperature
  lcd.setCursor(16, 2); // row 2 col 16
  lcd.print((int)DHT.temperature);
  lcd.write(0xDF);
  lcd.print("C");

 int speed2 = analogRead(potPin2) / 4;
  boolean reverse2 = digitalRead(switchPin2);
  setMotor2(speed2, reverse2);
  
  if(DHT.temperature >= 30)
  {
    Serial.print("  \t\t");
   // Serial.print(" ");
    Serial.println("Fan is ON");
    //digitalWrite(in2Pin2,HIGH);
   digitalWrite(enablePin2,HIGH);
   
    digitalWrite(in1Pin2,HIGH);
  }
  else
  {
    Serial.print("  \t\t");
   // Serial.print(" ");
    Serial.println("Fan is OFF");
    digitalWrite(in1Pin2,LOW);
  }
/********* TEMPERATURE CONTROL SYSTEM END  ************/
   delay (2000); 
}

void setMotor(int speed,boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin,  reverse);
  digitalWrite(pinOut, HIGH);
}

 void setMotor2(int speed2, boolean reverse2)
{
  analogWrite(enablePin2, speed2);
  digitalWrite(in1Pin2, ! reverse2);
  digitalWrite(in2Pin2, reverse2);
}
