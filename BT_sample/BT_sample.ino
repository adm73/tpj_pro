#include <SoftwareSerial.h>


SoftwareSerial BT(50, 52); // Uno onBoard: 10 rx, 9 tx; Mega 2560: 14 tx, 15 rx


char ch;

int is_SW_FAN    = 0;
int is_SW_COVER  = 0;
int is_SW_HEATER = 0;
int is_SW_WPUMP  = 0;

int is_TR_HUM = 0;
int is_TR_BRT = 0;
int is_TR_TMP = 0;


int hum = 355;
int brt = 667;
int tmp = 330;

#define HUM_MAX_VALUE  80
#define HUM_MIN_VALUE  50

#define BRT_MAX_VALUE  70

#define TMP_MAX_VALUE  30
#define TMP_MIN_VALUE  20


int max_hum = HUM_MAX_VALUE;
int min_hum = HUM_MIN_VALUE;
int cur_hum = 0;

int max_temp = TMP_MAX_VALUE;
int min_temp = TMP_MIN_VALUE;
int cur_temp = 0;

int max_brt = BRT_MAX_VALUE;
int cur_brt = 0;


#define LOCK_HEATER 0
#define LOCK_COVER  1
#define LOCK_FAN  2
#define LOCK_WPUMP 3

int islocked[4] = {0};

void led_control(int r, int y, int g)
{

}

void hum_set_max_value(int __max)
{
  max_hum = __max;
}

void hum_set_min_value(int __min)
{
  min_hum = __min;
}

void brt_set_max_value(int __max)
{
  max_brt = __max;
}

void temp_set_max_value(int __max)
{
  max_temp = __max;
}

void temp_set_min_value(int __min)
{
  min_temp = __min;
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

void enable_wpump(void)
{
  Serial.println("enable_wpump");
}

void disable_wpump(void)
{
  Serial.println("disable_wpump");
}

void enable_fan(void)
{
  Serial.println("enable_fan");
}

void disable_fan(void)
{
    Serial.println("disable_fan");
}

void heater_enable(void)
{
  Serial.println("heater_enable");
}

void heater_disable(void)
{
  Serial.println("heater_disable");
}

int get_hum(void)
{
  return hum;
}

void set_hum(int __hum)
{
  hum = __hum;
}

int get_brt(void)
{
  return brt;
}

void set_brt(int __brt)
{
  brt = __brt;
}

int get_tmp(void)
{
  return tmp;
}

void set_tmp(int __tmp)
{
  tmp = __tmp;
}

void bt_setup(void)
{
  BT.begin(9600);

  BT.println("DT @ House");
}

void bt_send_msg(char *prefix, int value)
{
  char str[16] = "\0";

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

static int is_coveropen = 0;

void cover_operate(int on)
{
  if(on == 1)
  {
    if(is_coveropen == 1)
      return ;
    Serial.println("cover_operate ON");
    is_coveropen = 1;
  }
  else
  {
    if(is_coveropen == 0)
      return ;
    Serial.println("cover_operate OFF");
    is_coveropen = 0;
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

void fan_operate(int on)
{
  if(on == 1)
    enable_fan();
  else
    disable_fan();
}

void wpump_operate(int on)
{
  if(on == 1)
    enable_wpump();
  else
    disable_wpump();
}


void set_trigger_value(int value)
{
  Serial.print("max_hum = ");
  Serial.println(max_hum);

  Serial.println("set_trigger_value begin");
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
  
  Serial.print("max_hum = ");
  Serial.println(max_hum);

  Serial.println("set_trigger_value end");
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);
  
  Serial.println("Hello, World");

  bt_setup();
  
  islocked[LOCK_HEATER] = 0;    // Heater
  islocked[LOCK_COVER] = 0;    // Cover
  islocked[LOCK_FAN] = 0;    // Fan
  islocked[LOCK_WPUMP] = 0;    // Wpump


}

void loop()
{
  // Humidity
  // bt_send_msg(" HUM,", get_hum());
  delay(200);

  // Brightness
  // bt_send_msg(" BRT,", get_brt());
  delay(200);

  // Temperature
  // bt_send_msg(" TMP,", get_tmp());
  delay(200);

  // put your main code here, to run repeatedly:
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
        {
          
          islocked[LOCK_HEATER] = 1;
        }
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
        {
          
          islocked[LOCK_COVER] = 1;
        }
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
        {
          
          islocked[LOCK_COVER] = 1;
        }
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
        {
          
          islocked[LOCK_FAN] = 1;
        }
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
        {
          
          islocked[LOCK_FAN] = 1;
        }
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
        {
          
          islocked[LOCK_WPUMP] = 1;
        }
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
        {
          
          islocked[LOCK_WPUMP] = 1;
        }
        else
          islocked[LOCK_WPUMP] = 0;
          
       break;

      // Soil Moisture Sensor Triggered
      case 's':
        is_TR_HUM = 1;  // min
        Serial.println("is_TR_HUM min");
        break;

      case 'S':
        is_TR_HUM = 2;  // max
        Serial.println("is_TR_HUM max");
        break;

      // Light Sensor Triggered
      case 'b':
        is_TR_BRT = 1;  // min
        Serial.println("is_TR_BRT min");
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


  if(cur_hum < min_hum)   // Wet
  {
    led_control(0, 0, 1);  // r, y, g

    // Disable Water Pump
    if(!islocked[LOCK_WPUMP]) wpump_operate(0);
  }
  else if((cur_hum >= min_hum) && (cur_hum < max_hum)) // Normal
  {
    led_control(0, 1, 0);  // r, y, g

    // Disable Water Pump
    if(!islocked[LOCK_WPUMP]) wpump_operate(0);
  }
  else  //  > max_hum = dry
  {
    led_control(1, 0, 0);  // r, y, g

    // Enable Water Pump
    if(!islocked[LOCK_WPUMP]) wpump_operate(1);
  }
  
  ch = 0x0;

  delay(2000);
}
