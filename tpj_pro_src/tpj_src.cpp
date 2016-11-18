

#include <SoftwareSerial.h>     // for Bluetooth

#include <LiquidCrystal_I2C.h>  // for LCD
#include <Wire.h>

#include <dht.h>    // for Temperature Sensor
#include <Servo.h>  // for Brightness  Sensor

SoftwareSerial BT(14, 15); // Uno on Board: 9 rx, 10 tx; Mega 2560: 14 tx, 15 rx
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

// TEMP SENSOR
int PIN_TEMP_SENSOR = A5;

// BRT SENSOR
int PIN_BRT_SENSOR_1 = A2;
int PIN_BRT_SENSOR_2 = A3;

// SOIL MOISTURE SENSOR
int PIN_HUM_SENSOR = A4;

// Heater
int PIN_HEATER_OUTPUT = 22;

// COVER
int PIN_SERVO_1 = 7;
int PIN_SERVO_2 = 6;
int PIN_SERVO_3 = 28;
int PIN_SERVO_4 = 30;

// WATER PUMP
int PIN_WPUMP_ENABLE = 12; // enable
int PIN_WPUMP_IN = 11;    // in
int PIN_WPUMP_RELAY = 13;    // for relay
int PIN_WPUMP_SPEED = A0;    // speed

// LED
int PIN_LED_GREEN = 10;
int PIN_LED_YELLOW = 9;
int PIN_LED_RED = 8;

// Fan Motor
int PIN_FAN_MOTOR_ENABLE = 5; // pin enable
int PIN_FAN_MOTOR_IN = 4;    // pin in
int PIN_FAN_MOTOR_SPEED = A1;  // speed

#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_ANGLE 10

#define HUM_MAX_VALUE  800
#define HUM_MIN_VALUE  500

#define BRT_MAX_VALUE  700

#define TMP_MAX_VALUE  30
#define TMP_MIN_VALUE  16

#define LOCK_HEATER 0
#define LOCK_COVER  1
#define LOCK_FAN  2
#define LOCK_WPUMP 3

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int is_SW_FAN    = 0;
int is_SW_COVER  = 0;
int is_SW_HEATER = 0;
int is_SW_WPUMP  = 0;

int is_TR_HUM = 0;
int is_TR_BRT = 0;
int is_TR_TMP = 0;


int max_hum = HUM_MAX_VALUE;
int min_hum = HUM_MIN_VALUE;
int cur_hum = 0;

int max_temp = TMP_MAX_VALUE;
int min_temp = TMP_MIN_VALUE;
int cur_temp = 0;

int max_brt = BRT_MAX_VALUE;
int cur_brt = 0;

int islocked[4] = {0};


void hum_set_max_value(int __max)
{
  max_hum = __max;
}

void hum_set_min_value(int __min)
{
  min_hum = __min;
}

void hum_set_value(int __hum)
{
  Serial.println("Use set_max_value or set_min_value");;
}

int hum_get_value(void)
{
  return analogRead(PIN_HUM_SENSOR);
}

void brt_set_max_value(int __max)
{
  max_brt = __max;
}

void brt_set_value(int __brt)
{
  Serial.println("Use set_max_value or set_min_value");
}

int brt_get_value(void)
{
  int v1, v2;
  int ret;

  v1 = brt_get_sensor1();
  v2 = brt_get_sensor2();

  ret = (v1 + v2) / 2;

  return ret;
}

void temp_set_max_value(int __max)
{
  max_temp = __max;
}

void temp_set_min_value(int __min)
{
  min_temp = __min;
}

void temp_set_value(int __tmp)
{
  Serial.println("Use set_max_value or set_min_value");
}

int temp_get_value(void)
{
  dht DHT;
  DHT.read11(PIN_TEMP_SENSOR);

  return ((int)DHT.temperature);
}

void lcd_display_hum(int __hum)
{
    lcd.setCursor(1, 2); // 0 1
    lcd.print(__hum);
    lcd.print("     ");
}

void lcd_display_brt(int __brt)
{
    lcd.setCursor(8, 2); // row 2 col 8
    lcd.print(__brt);
    lcd.print("     ");
}

void lcd_display_tmp(int __tmp)
{
    lcd.setCursor(16, 2); // row 2 col 16
    lcd.print(__tmp);
    lcd.write(0xDF);
    lcd.print("C");
}

void lcd_myinit(void)
{
  lcd.init();
  lcd.backlight();
  lcd.clear();

  delay(300);

  lcd.setCursor(0, 0); // col row 0 0
  lcd.print(" HUM    BRT    TEMP ");

  delay(300);
}

void led_control(int r, int y, int g)
{
  if(r)
    digitalWrite(PIN_LED_RED, HIGH);
  else
    digitalWrite(PIN_LED_RED, LOW);

  if(y)
    digitalWrite(PIN_LED_YELLOW, HIGH);
  else
    digitalWrite(PIN_LED_YELLOW, LOW);

  if(g)
    digitalWrite(PIN_LED_GREEN, HIGH);
  else
    digitalWrite(PIN_LED_GREEN, LOW);
}

void led_myinit(void)
{
  pinMode(PIN_LED_GREEN, OUTPUT); // green 10
  pinMode(PIN_LED_YELLOW, OUTPUT); // yellow 9
  pinMode(PIN_LED_RED, OUTPUT); // red 8
}

void serial_myinit()
{
  Serial.begin(9600);

  Serial.println("Hello, World!");
}

int brt_get_sensor1(void)
{
  return analogRead(PIN_BRT_SENSOR_1);
}

int brt_get_sensor2(void)
{
  return analogRead(PIN_BRT_SENSOR_2);
}


int brt_shouldopen(void)
{
  int v1, v2;

  v1 = brt_get_sensor1();
  v2 = brt_get_sensor2();

  if((v1 >= max_brt) && (v2 >= max_brt))
    return 1;

  return 0;
}

void brt_myinit(void)
{
  pinMode(PIN_BRT_SENSOR_1, INPUT);     // A2
  pinMode(PIN_BRT_SENSOR_2, INPUT);     // A3
}

void servo_cover_open(void)
{
  servo1.write(SERVO_MIN_ANGLE);
  servo2.write(SERVO_MIN_ANGLE);

  servo3.write(SERVO_MAX_ANGLE);
  servo4.write(SERVO_MAX_ANGLE);
}

void servo_cover_close(void)
{
  servo1.write(SERVO_MAX_ANGLE);
  servo2.write(SERVO_MAX_ANGLE);

  servo3.write(SERVO_MIN_ANGLE);
  servo4.write(SERVO_MIN_ANGLE);
}

void servo_myinit(void)
{
  servo1.attach(PIN_SERVO_1);                   // pin 7
  servo2.attach(PIN_SERVO_2);                   // pin 6
  servo3.attach(PIN_SERVO_3);                   // pin 28
  servo4.attach(PIN_SERVO_4);                   // pin 30
}

void temp_myinit(void)
{
  pinMode(PIN_TEMP_SENSOR, INPUT);
}

void enable_wpump(void)
{
  digitalWrite(PIN_WPUMP_IN,  HIGH);
  digitalWrite(PIN_WPUMP_RELAY, HIGH);
}

void disable_wpump(void)
{
  digitalWrite(PIN_WPUMP_IN,  LOW);
  digitalWrite(PIN_WPUMP_RELAY, LOW);
}

void wpump_motor_myinit(void)
{
  pinMode(PIN_WPUMP_ENABLE, OUTPUT); // pin 12
  pinMode(PIN_WPUMP_IN, OUTPUT); // pin 11

  // for Relay
  pinMode(PIN_WPUMP_RELAY, OUTPUT); // pin 13

  int speed = analogRead(PIN_WPUMP_SPEED) / 4;
  analogWrite(PIN_WPUMP_ENABLE, speed);

//  Serial.print("speed = ");
//  Serial.print(speed);
//  Serial.print("\n");
}

void enable_fan(void)
{
  digitalWrite(PIN_FAN_MOTOR_IN, HIGH);
}

void disable_fan(void)
{
  digitalWrite(PIN_FAN_MOTOR_IN, LOW);
}

void fan_motor_myinit(void)
{
  pinMode(PIN_FAN_MOTOR_IN, OUTPUT); // pin 4
  pinMode(PIN_FAN_MOTOR_ENABLE, OUTPUT); // pin 5

  int speed = analogRead(PIN_FAN_MOTOR_SPEED) / 4;
  analogWrite(PIN_FAN_MOTOR_ENABLE, speed);

//  Serial.println("Fan speed = ");
//  Serial.println(speed);
}


void heater_enable(void)
{
  digitalWrite(PIN_HEATER_OUTPUT, HIGH);
}

void heater_disable(void)
{
  digitalWrite(PIN_HEATER_OUTPUT, LOW);
}

void heater_myinit(void)
{
  pinMode(PIN_HEATER_OUTPUT, OUTPUT);
}

void bt_send_msg(char *prefix, int value)
{
  char str[16];

  memset(str, 0, sizeof(str));

  sprintf(str, "%s%d", prefix, value);

  BT.println(str);
}

char bt_get_msg(void)
{
  if(BT.available())
    return (BT.read());
}

void bt_myinit(void)
{
  BT.begin(9600);

  BT.println("Hello, World!");
}

void tr_set_hum(void)
{
        Serial.println("change humidity");
        if(is_TR_HUM == 0)
          is_TR_HUM = 1;
}

void tr_set_brt(void)
{
        Serial.println("change brightness");
        if(is_TR_BRT == 0)
          is_TR_BRT = 1;
}

void tr_set_tmp(void)
{
        Serial.println("change temp");
        if(is_TR_TMP == 0)
          is_TR_TMP = 1;
}

void oper_sw_heater(void)
{
        Serial.println("Heater Switch Operate");
        if(is_SW_HEATER == 0)
        {
          is_SW_HEATER = 1;
          Serial.println("Heater ON");

          // heater on
        }
        else
        {
          is_SW_HEATER = 0;
          Serial.println("Heater OFF");

          // heater off
        }
}

void oper_sw_cover(void)
{
        Serial.println("Cover Switch Operate");
        if(is_SW_COVER == 0)
        {
          is_SW_COVER = 1;
          Serial.println("Cover ON");

          // cover on
        }
        else
        {
          is_SW_COVER = 0;
          Serial.println("Cover OFF");

          // cover off
        }
}

void oper_sw_fan(void)
{
        oper_sw_fan();
        Serial.println("Fan Switch Operate");
        if(is_SW_FAN == 0)
        {
          is_SW_FAN = 1;
          Serial.println("Fan ON");

          // fan on
        }
        else
        {
          is_SW_FAN = 0;
          Serial.println("Fan OFF");

          // fan off
        }
}

void oper_sw_wpump(void)
{
        Serial.println("Water Pump Operate");
        if(is_SW_WPUMP == 0)
        {
          is_SW_WPUMP = 1;
          Serial.println("Water Pump ON");

          // wpump on
          enable_wpump();
        }
        else
        {
          is_SW_WPUMP = 0;
          Serial.println("Water Pump OFF");

          // wpump off
          disable_wpump();
        }
}

void cover_operate(int on)
{
  if(islocked[LOCK_COVER] == 1)
    return ;

  if(on == 1)
    servo_cover_open();
  else
    servo_cover_close();
}

void heater_operate(int on)
{
  if(islocked[LOCK_HEATER] == 1)
    return ;

  if(on == 1)
    heater_enable();
  else
    heater_disable();
}

void fan_operate(int on)
{
  if(islocked[LOCK_FAN] == 1)
    return ;

  if(on == 1)
    enable_fan();
  else
    disable_fan();
}

void wpump_operate(int on)
{
  if(islocked[LOCK_WPUMP] == 1)
    return ;

  if(on == 1)
    enable_wpump();
  else
    disable_wpump();
}


void set_trigger_value(int value)
{
  switch(is_TR_TMP)
  {
    case 1: // min
      temp_set_min_value(value * 8);
      is_TR_TMP = 0;
      break;

    case 2: // max
      temp_set_max_value(value * 8);
      is_TR_TMP = 0;
      break;

    default:
      break;
  }

  switch(is_TR_BRT)
  {
    case 1:
    case 2:
      brt_set_max_value(value * 8);
      is_TR_BRT = 0;
      break;

    default:
      break;
  }

  switch(is_TR_HUM)
  {
    case 1:
      hum_set_min_value(value * 8);
      is_TR_HUM = 0;
      break;

    case 2:
      hum_set_max_value(value * 8);
       is_TR_HUM = 0;
     break;

    default:
      break;
  }

}

void setup()
{
  // Serial
  serial_myinit();

  // Bluetooth
  bt_myinit();

  // LCD
  lcd_myinit();

  // LEDs
  led_myinit();

  // Light Sensor
  brt_myinit();

  // Servo
  servo_myinit();

  // Temperature
  temp_myinit();

  // Water Pump
  wpump_motor_myinit();

  // Fan Motor
  fan_motor_myinit();

  // Heater
  heater_myinit();


  // Display Welcome Message on Serial
  // Serial.println("-------------------------------------------------------------------------------------------------------------------------------------------");
  // Serial.println("Light Sensor 1 \t Light Sensor 2 \t Soil Moisture \t Cover Status \t Temperature \t Humidity \t Fan Status");

  islocked[LOCK_HEATER] = 0;    // Heater
  islocked[LOCK_COVER] = 0;    // Cover
  islocked[LOCK_FAN] = 0;    // Fan
  islocked[LOCK_WPUMP] = 0;    // Wpump


  delay (2000);
}

char ch;

void loop()
{

  // get Brightness
  // brt_set_value(brt_get_value());
  lcd_display_brt(brt_get_value());
  bt_send_msg(" BRT,", brt_get_value());
  delay(200);

  // Get Humidity
  // hum_set_value(hum_get_value());
  lcd_display_hum(hum_get_value());
  bt_send_msg(" HUM,", hum_get_value());
  delay(200);

  // Get Temperature
  // temp_set_value(temp_get_value());
  lcd_display_tmp(temp_get_value());
  bt_send_msg(" TMP,", temp_get_value());
  delay(200);

  ch = bt_get_msg();
  switch(ch)
    {
      // Heater Switch Operate
      case 'H':
        if(islocked[LOCK_HEATER] == 0)
        {
          heater_operate(1);
          islocked[LOCK_HEATER] = 1;
        }
        else
          islocked[LOCK_HEATER] = 0;
        break;

      case 'h':
        if(islocked[LOCK_HEATER] == 0)
        {
          heater_operate(0);
          islocked[LOCK_HEATER] = 1;
        }
        else
          islocked[LOCK_HEATER] = 0;
       break;

      // Cover Switch Operate
      case 'C':
        if(islocked[LOCK_COVER] == 0)
        {
          cover_operate(1);
          islocked[LOCK_COVER] = 1;
        }
        else
          islocked[LOCK_COVER] = 0;
        break;

      case 'c':
        if(islocked[LOCK_COVER] == 0)
        {
          cover_operate(0);
          islocked[LOCK_COVER] = 1;
        }
        else
          islocked[LOCK_COVER] = 0;
        break;

      // Fan Switch Operate
      case 'F':
        if(islocked[LOCK_FAN] == 0)
        {
          fan_operate(1);
          islocked[LOCK_FAN] = 1;
        }
        else
          islocked[LOCK_FAN] = 0;
       break;

      case 'f':
        if(islocked[LOCK_FAN] == 0)
        {
          fan_operate(0);
          islocked[LOCK_FAN] = 1;
        }
        else
          islocked[LOCK_FAN] = 0;
        break;

      // Water Pump Operate
      case 'W':
        if(islocked[LOCK_WPUMP] == 0)
        {
          wpump_operate(1);
          islocked[LOCK_WPUMP] = 1;
        }
        else
          islocked[LOCK_WPUMP] = 0;
        break;

      case 'w':
        if(islocked[LOCK_WPUMP] == 0)
        {
          wpump_operate(0);
          islocked[LOCK_WPUMP] = 1;
        }
        else
          islocked[LOCK_WPUMP] = 0;
       break;

      // Soil Moisture Sensor Triggered
      case 's':
        is_TR_HUM = 1;  // min
        break;

      case 'S':
        is_TR_HUM = 2;  // max
        break;

      // Light Sensor Triggered
      case 'b':
        is_TR_BRT = 1;  // min
        break;

      case 'B':
        is_TR_BRT = 2;  // max
        break;

      // Temperature Sensor Triggered
      case 't':
        is_TR_TMP = 1;  // min
        break;

      case 'T':
        is_TR_TMP = 2;  // max
        break;

      default:
        set_trigger_value(ch);
        break;
    }

  if(brt_shouldopen() == 1)
  {
    Serial.print("Cover OPEN");
    cover_operate(1);

  }
  else
  {
    Serial.print("Cover Close");
    cover_operate(0);
  }

  if(hum_get_value() < min_hum)   // Wet
  {
    led_control(0, 0, 1);  // r, y, g

    // Disable Water Pump
    wpump_operate(0);
  }
  else if((hum_get_value() >= min_hum) && (hum_get_value() < max_hum)) // Normal
  {
    led_control(0, 1, 0);  // r, y, g

    // Disable Water Pump
    wpump_operate(0);
  }
  else  //  > max_hum = dry
  {
    led_control(1, 0, 0);  // r, y, g

    // Enable Water Pump
    wpump_operate(1);
  }

  if(temp_get_value() >=  max_temp) // Hot
  {
    // Fan on
    Serial.println("Fan ON, Heater OFF");
    fan_operate(1);

    // Heater OFF
    heater_operate(0);
  }
  else if((temp_get_value() < max_temp) && (temp_get_value() >= min_temp)) // Normal
  {
    // Fan OFF
    Serial.println("Fan OFF, Heater OFF");
    fan_operate(0);

    // Heater OFF
    heater_operate(0);
  }
  else // Cold
  {
    //Fan off
    Serial.println("Fan OFF, Heater ON");
    fan_operate(0);

    // Heater ON
    heater_operate(1);
  }

   delay (2000);
}
