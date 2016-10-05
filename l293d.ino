int enablePin = 11;
int in1Pin = 10;
//int in2Pin = 9;
int pinOut = 9;
//int switchPin = 7;
int potPin = 0;
int statusPin= 13;
int soilPin = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(in1Pin, OUTPUT); //pin 10
  //pinMode(in2Pin, OUTPUT);
  pinMode(pinOut, OUTPUT); //pin 9
  pinMode(enablePin, OUTPUT); //pin 11
  //pinMode(switchPin, INPUT_PULLUP); //pin 7
  pinMode(statusPin,OUTPUT); //pin 13 // not in use
 // pinMode(10, OUTPUT);
}

int soilValue;

void loop()
{
  digitalWrite(13,HIGH);
  int speed = analogRead(potPin) / 4;

  soilValue = analogRead(soilPin);
  //boolean reverse = digitalRead(switchPin);
  boolean reverse = digitalRead(pinOut);

  Serial.println(soilValue);
  if(soilValue > 700)
    setMotor(speed, reverse);
  else
    setMotor(1, reverse);
}

void setMotor(int speed,boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin,  reverse);
  digitalWrite(pinOut, HIGH);
}
