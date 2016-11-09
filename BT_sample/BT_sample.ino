#include <SoftwareSerial.h>


SoftwareSerial BT(12,13); // rx, tx


char a;
void setup() {
  // put your setup code here, to run once:
  BT.begin(9600); 
  
  BT.println("Yuchen from Arduino");
}

void loop() {
  
  BT.println("yuchen from Arduino");
  // put your main code here, to run repeatedly:
  if (BT.available())
  // if text arrived in from BT serial...
  {
    a=(BT.read());
    if (a=='1')
    {
      digitalWrite(13, HIGH);
      BT.println("LED on");
    }
    if (a=='2')
    {
      digitalWrite(13, LOW);
      BT.println("LED off");
    }
    if (a=='?')
    {
      BT.println("Send '1' to turn LED on");
      BT.println("Send '2' to turn LED on");
    }   
    // you can add more "if" statements with other characters to add more commands
  }
}
