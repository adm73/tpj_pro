

#include <SoftwareSerial.h>     // for Bluetooth

#include <LiquidCrystal_I2C.h>  // for LCD
#include <Wire.h>

#include <dht.h>    // for Temperature Sensor
#include <Servo.h>  // for Brightness  Sensor

SoftwareSerial BT(50, 52); // Uno on Board: 9 rx, 10 tx; Mega 2560: 14 tx, 15 rx
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
int PIN_FAN_RELAY =24;
int PIN_WPUMP_SPEED = A0;    // speed

// LED
int PIN_LED_GREEN = 10;
int PIN_LED_YELLOW = 9;
int PIN_LED_RED = 8;

// Fan Motor
int PIN_FAN_MOTOR_ENABLE = 2; // pin enable
int PIN_FAN_MOTOR_IN = 3;    // pin in
int PIN_FAN_MOTOR_SPEED = A1;  // speed

#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_ANGLE 10

#define HUM_MAX_VALUE  80
#define HUM_MIN_VALUE  50

#define BRT_MAX_VALUE  70

#define TMP_MAX_VALUE  30
#define TMP_MIN_VALUE  20

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
  cur_hum = __hum;
}

int hum_get_value(void)
{
  int hum = analogRead(PIN_HUM_SENSOR);

  if(hum < 0)
    hum = 0;
  else if(hum >= 1000)
    hum = 999;

  hum /= 10;

  hum = 100 - hum;

  return hum;
}

void brt_set_max_value(int __max)
{
  max_brt = __max;
}

void brt_set_value(int __brt)
{
  cur_brt = __brt;
}

int brt_get_value(void)
{
  int v1, v2;
  int ret;

  v1 = brt_get_sensor1();
  // Serial.print("brt1 = ");
  // Serial.println(v1);
  if(v1 < 0)
    v1 = 0;
  else if(v1 >= 1000)
    v1 = 999;

  v2 = brt_get_sensor2();
  // Serial.print("brt2 = ");
  // Serial.println(v2);

  if(v2 < 0)
    v2 = 0;
  else if(v2 >= 1000)
    v2 = 999;

  ret = (v1 + v2) / 2;

  ret /= 10;

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
  cur_temp = __tmp;
}

int temp_get_value(void)
{
  int temp;
  dht DHT;
  DHT.read11(PIN_TEMP_SENSOR);

  temp = (int)DHT.temperature;

  // Serial.println(DHT.temperature);
  if(temp >= 40)
    temp = 36;
  else if(temp < 0)
    temp = 0;

  return temp;
}

void lcd_display_hum(int __hum)
{
    lcd.setCursor(1, 2); // 0 1
    lcd.print(__hum);
    lcd.print("%    ");
}

void lcd_display_brt(int __brt)
{
    lcd.setCursor(8, 2); // row 2 col 8
    lcd.print(__brt);
    lcd.print("%    ");
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

  Serial.println("Hello, Arduino!");
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

  v1 /= 10;
  v2 /= 10;

  if((v1 >= max_brt) && (v2 >= max_brt))
//    if(((v1 * v1) + (v2 * v2)) > ((v1 + v2) * (v1 + v2)))
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
}

void enable_fan(void)
{
  digitalWrite(PIN_FAN_MOTOR_IN, HIGH);
  digitalWrite(PIN_FAN_RELAY, HIGH);
  
}

void disable_fan(void)
{
  digitalWrite(PIN_FAN_MOTOR_IN, LOW);
  digitalWrite(PIN_FAN_RELAY, LOW);
 
}

void fan_motor_myinit(void)
{
  pinMode(PIN_FAN_MOTOR_IN, OUTPUT); // pin 4
  pinMode(PIN_FAN_MOTOR_ENABLE, OUTPUT); // pin 5
  pinMode(PIN_FAN_RELAY, OUTPUT); //FAN RELAY
  int speed = analogRead(PIN_FAN_MOTOR_SPEED) / 4;
  analogWrite(PIN_FAN_MOTOR_ENABLE, speed);

}


void heater_enable(void)
{
  digitalWrite(PIN_HEATER_OUTPUT, LOW);
}

void heater_disable(void)
{
  digitalWrite(PIN_HEATER_OUTPUT, HIGH);
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
  char ch;

  ch = 0x0;
  if(BT.available())
  {
    ch = BT.read();

    if(ch < 0)
      ch = 0x0;

    if(ch == 0xD || ch == 0xA)
      ch = 0x0;

  }

  return ch;
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

static int is_cover_on = 0;

void cover_operate(int on)
{
  if(on == 1)
  {
    if(is_cover_on == 1)
      return ;
    servo_cover_open();
    is_cover_on = 1;
  }
  else
  {
    if(is_cover_on == 0)
      return ;

    servo_cover_close();
    is_cover_on = 0;
  }
}

static int is_heater_on = 0;

void heater_operate(int on)
{
  if(on == 1)
  {
    if(is_heater_on == 1)
      return ;
    heater_enable();
    is_heater_on = 1;
  }
  else
  {
    if(is_heater_on == 0)
      return ;

    heater_disable();
    is_heater_on = 0;
  }
}

static int is_fan_on = 0;

void fan_operate(int on)
{
  // return ;

  if(on == 1)
  {
    if(is_fan_on == 1)
      return ;
    enable_fan();
    is_fan_on = 1;
  }
  else
  {
    if(is_fan_on == 0)
      return ;
    disable_fan();
    is_fan_on = 0;
  }
}

static int is_wpump_on = 0;

void wpump_operate(int on)
{
  if(on == 1)
  {
    if(is_wpump_on == 1)
      return ;
    enable_wpump();
    is_wpump_on = 1;
  }
  else
  {
    if(is_wpump_on == 0)
      return;
    disable_wpump();
    is_wpump_on = 0;
  }
}


void set_trigger_value(int value)
{
  switch(is_TR_TMP)
  {
    case 1: // min
      temp_set_min_value(value);
      is_TR_TMP = 0;
      break;

    case 2: // max
      temp_set_max_value(value);
      is_TR_TMP = 0;
      break;

    default:
      break;
  }

  switch(is_TR_BRT)
  {
    case 1:
    case 2:
      brt_set_max_value(value);
      is_TR_BRT = 0;
      break;

    default:
      break;
  }

  switch(is_TR_HUM)
  {
    case 1:
      hum_set_min_value(value);
      is_TR_HUM = 0;
      break;

    case 2:
      hum_set_max_value(value);
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

static int count = 5;

void loop()
{
  // get Brightness
  brt_set_value(brt_get_value());

  if(count > 4)
  {
    lcd_display_brt(cur_brt);
    bt_send_msg(" BRT,", cur_brt);
  }

  delay(200);

  // Get Humidity
  hum_set_value(hum_get_value());
  if(count > 4)
  {
    lcd_display_hum(cur_hum);
    bt_send_msg(" HUM,", cur_hum);
  }

  delay(200);


  // Get Temperature
  temp_set_value(temp_get_value());

  if(count > 4)
  {
    lcd_display_tmp(cur_temp);
    bt_send_msg(" TMP,", cur_temp);

    count = 0;
  }

  delay(200);

  ch = 0x0;
  ch = bt_get_msg();
  if(ch)
  {
    Serial.println(ch);
    bt_send_msg(" RECV,", ch);

    switch(ch)
    {
      // Heater Switch Operate
      case 'H':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }


        heater_operate(1);
        if(islocked[LOCK_HEATER] == 0)
          islocked[LOCK_HEATER] = 1;
        else
          islocked[LOCK_HEATER] = 0;
        break;

      case 'h':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }

        heater_operate(0);
        if(islocked[LOCK_HEATER] == 0)
          islocked[LOCK_HEATER] = 1;
        else
          islocked[LOCK_HEATER] = 0;
       break;

      // Cover Switch Operate
      case 'C':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }

        cover_operate(1);
        if(islocked[LOCK_COVER] == 0)
          islocked[LOCK_COVER] = 1;
        else
          islocked[LOCK_COVER] = 0;
        break;

      case 'c':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }

        cover_operate(0);
        if(islocked[LOCK_COVER] == 0)
          islocked[LOCK_COVER] = 1;
        else
          islocked[LOCK_COVER] = 0;
        break;

      // Fan Switch Operate
      case 'F':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }

        fan_operate(1);
        if(islocked[LOCK_FAN] == 0)
          islocked[LOCK_FAN] = 1;
        else
          islocked[LOCK_FAN] = 0;
       break;

      case 'f':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }

        fan_operate(0);
        if(islocked[LOCK_FAN] == 0)
          islocked[LOCK_FAN] = 1;
        else
          islocked[LOCK_FAN] = 0;
        break;

      // Water Pump Operate
      case 'W':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }

        wpump_operate(1);
        if(islocked[LOCK_WPUMP] == 0)
          islocked[LOCK_WPUMP] = 1;
        else
          islocked[LOCK_WPUMP] = 0;
        break;

      case 'w':
        if(is_TR_HUM || is_TR_BRT || is_TR_TMP)
        {
          set_trigger_value(ch);
          break;
        }

        wpump_operate(0);
        if(islocked[LOCK_WPUMP] == 0)
          islocked[LOCK_WPUMP] = 1;
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

  }

  #if 1

  if(brt_shouldopen() == 1)
  {
    // Serial.println("Cover OPEN");
    if(!islocked[LOCK_COVER]) cover_operate(1);

  }
  else
  {
    // Serial.println("Cover Close");
    if(!islocked[LOCK_COVER]) cover_operate(0);
  }

  if(cur_hum < min_hum) //  > max_hum = dry
  {
    led_control(1, 0, 0);  // r, y, g

    // Enable Water Pump
    if(!islocked[LOCK_WPUMP]) wpump_operate(1);

      }
  else if((cur_hum >= min_hum) && (cur_hum < max_hum)) // Normal
  {
    led_control(0, 1, 0);  // r, y, g

    // Disable Water Pump
    if(!islocked[LOCK_WPUMP]) wpump_operate(0);
  }
  else    // Wet
  {
    led_control(0, 0, 1);  // r, y, g

    // Disable Water Pump
    if(!islocked[LOCK_WPUMP]) wpump_operate(0);
  }

  if(cur_temp >=  max_temp) // Hot
  {
    // Fan on
    // Serial.println("Fan ON, Heater OFF");

    if(!islocked[LOCK_FAN]) fan_operate(1);

    // Heater OFF
    if(!islocked[LOCK_HEATER]) heater_operate(0);
  }
  else if((cur_temp < max_temp) && (cur_temp >= min_temp)) // Normal
  {
    // Fan OFF
    // Serial.println("Fan OFF, Heater OFF");
    if(!islocked[LOCK_FAN]) fan_operate(0);

    // Heater OFF
    if(!islocked[LOCK_HEATER]) heater_operate(0);
  }
  else // Cold
  {
    //Fan off
    // Serial.println("Fan OFF, Heater ON");
    if(!islocked[LOCK_FAN]) fan_operate(0);

    // Heater ON
    if(!islocked[LOCK_HEATER]) heater_operate(1);
  }

  #endif

#if 0
  Serial.println("Status");
  Serial.print("is_heater_on = "); //is_wpump_on
  Serial.println(is_heater_on);

  Serial.print("is_wpump_on = ");
  Serial.println(is_wpump_on);

  Serial.print("is_fan_on = ");
  Serial.println(is_fan_on);

  Serial.print("is_cover_on = ");
  Serial.println(is_cover_on);

  Serial.print("cur_temp = ");
  Serial.println(cur_temp);

  Serial.print("max_temp = ");
  Serial.println(max_temp);

  Serial.print("cur_hum = ");
  Serial.println(cur_hum);

  Serial.print("max_hum = ");
  Serial.println(max_hum);

  Serial.print("cur_brt = ");
  Serial.println(cur_brt);

  Serial.print("max_brt = ");
  Serial.println(max_brt);
#endif

  count++;

  delay (2000);
}

