
#include <Servo.h>

/**************** PIN ASSIGNMENT ******************/

// light sensor
#define PIN_LDR_INPUT 3
#define PIN_LDR_OUTPUT_RED 8
#define PIN_LDR_OUTPUT_GREEN 9

// servo
#define PIN_SERVO 10
/**************************************************/




/**************** Light Sensor ********************/

#define LDR_SENSITIVITY 500
#define LDR_OPEN_SERVO  1
#define LDR_CLOSE_SERVO 2

int Brightness = 0;

void LDR_init(void)
{
  pinMode(PIN_LDR_OUTPUT_GREEN, OUTPUT); 
  pinMode(PIN_LDR_OUTPUT_RED, OUTPUT);   
}

int LDR_read(void)
{
  return analogRead(PIN_LDR_INPUT);
}

int LDR_control(void)
{
  Brightness = LDR_read();
  
  Serial.println(Brightness);
  
  if(Brightness < LDR_SENSITIVITY)
  {
    Serial.println("Green on");
    digitalWrite(PIN_LDR_OUTPUT_GREEN, HIGH);
    digitalWrite(PIN_LDR_OUTPUT_RED, LOW);
    
    return LDR_OPEN_SERVO;
//    servo.write(10);
  }
  else
  {
    Serial.println("Red on");
    digitalWrite(PIN_LDR_OUTPUT_RED, HIGH);
    digitalWrite(PIN_LDR_OUTPUT_GREEN, LOW);
    
    return LDR_CLOSE_SERVO;
    
//    servo.write(120);
  }
}

/**************************************************/


/********************* Servo **********************/
Servo servo;

void Servo_init(void)
{
  servo.attach(PIN_SERVO); 
}

void Servo_control(void)
{
  
}
/**************************************************/


void setup() {
  // serial setup
  Serial.begin(9600);

  LDR_init();
  Servo_init();
}

void loop() {
  // light sensor control
  LDR_control();
  Servo_control();

  delay(500);
}
