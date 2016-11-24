
#include <SoftwareSerial.h>
const int RX_PIN = 50;
const int TX_PIN = 52;
SoftwareSerial serial(RX_PIN, TX_PIN);
char commandChar;
void setup ()
{
  serial.begin (9600);
  // andomSeed(analogRead(0));
  
  serial.println("hello");
}
void loop ()
{
  if(serial.available())
  {
  commandChar = serial.read();
  serial.println(commandChar);
  switch(commandChar)
    {
      case '*':
      serial.print(random(1000) + "#");
      break;
    }
  }
}
