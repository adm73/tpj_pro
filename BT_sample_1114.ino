#include <SoftwareSerial.h>


SoftwareSerial BT(10,9); // rx, tx


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

  BT.println("DT in House");
}

void send_msg(char *prefix, int value)
{
  char str[16] = "\0";

  sprintf(str, "%s%d", prefix, value);

  BT.println(str);
}

char get_msg(void)
{
  if(BT.available())
    return (BT.read());
}

void oper_sw_wpump(void)
{
        Serial.println("Water Pump Operate");
        if(is_SW_WPUMP == 0)
        {
          is_SW_WPUMP = 1;
          Serial.println("Water Pump ON");

          // wpump on
        }
        else
        {
          is_SW_WPUMP = 0;
          Serial.println("Water Pump OFF");

          // wpump off
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

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600);

  bt_setup();
}

void loop()
{
  // Humidity
  send_msg(" HUM,", get_hum());
  delay(200);

  // Brightness
  send_msg(" BRT,", get_brt());
  delay(200);

  // Temperature
  send_msg(" TMP,", get_tmp());
  delay(200);

  // put your main code here, to run repeatedly:
  ch = get_msg();
    switch(ch)
    {
      // Heater Switch Operate
      case 'H':
        oper_sw_heater();
        break;

      // Cover Switch Operate
      case 'C':
        oper_sw_cover();
        break;

      // Fan Switch Operate
      case 'F':
        break;

      // Water Pump Operate
      case 'W':
        oper_sw_wpump();
        break;

      // Soil Moisture Sensor Triggered
      case 'h':
        tr_set_hum();
        break;

      // Light Sensor Triggered
      case 'b':
        tr_set_brt();
        break;

      // Temperature Sensor Triggered
      case 't':
        tr_set_tmp();
        break;

      default:
        // the trigger value
        if(is_TR_TMP == 1)
        {
          ; // trigger value = ch *8;
          is_TR_TMP = 0;
          set_tmp(ch * 8);
          break;
        }

        if(is_TR_BRT == 1)
        {
          ; // trigger value = ch *8;
          is_TR_BRT = 0;
          set_brt(ch * 8);
          break;
        }

        if(is_TR_HUM == 1)
        {
          ; // trigger value = ch *8;
          is_TR_HUM = 0;
          set_hum(ch * 8);
          break;
        }


        break;
    }

  delay(1000);
}
