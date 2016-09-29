
#include <Servo.h>

/**************** PIN ASSIGNMENT ******************/

#define PIN_IN_LDR 3          // light sensor
#define PIN_IN_SOIL_SENSOR 0  // soil YL-69

#define PIN_OUT_LED_R    9
#define PIN_OUT_LED_G    8
#define PIN_OUT_LED_Y    10

#define PIN_OUTPUT_SERVO 10   // servo output 

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

//  LDR_init();
//  Servo_init();

  // green
  pinMode(8, OUTPUT); 
  
  //red
  pinMode(9, OUTPUT); 

  //red
  pinMode(10, OUTPUT); 

}

int readValue;
void loop() {
  // light sensor control
//  LDR_control();
//  Servo_control();

  readValue = analogRead(PIN_SOIL_SENSOR_A0);

  // higher dryer
  Serial.println(readValue);
  
  if(readValue < 500)
  {
    digitalWrite(PIN_LDR_OUTPUT_GREEN, HIGH);
    digitalWrite(PIN_LDR_OUTPUT_YELLOW, LOW);
    digitalWrite(PIN_LDR_OUTPUT_RED, LOW);
    
  }
  else if(readValue >= 500 && readValue < 1000)
  {
    digitalWrite(PIN_LDR_OUTPUT_YELLOW, HIGH);
    digitalWrite(PIN_LDR_OUTPUT_RED, LOW);
    digitalWrite(PIN_LDR_OUTPUT_GREEN, LOW);
    
  }
  else
  {
    digitalWrite(PIN_LDR_OUTPUT_RED, HIGH);
    digitalWrite(PIN_LDR_OUTPUT_YELLOW, LOW);
    digitalWrite(PIN_LDR_OUTPUT_GREEN, LOW);
  }

  delay(500);
}
