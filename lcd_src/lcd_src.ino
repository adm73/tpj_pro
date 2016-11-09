
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
 

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
 
void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.print("Hello, World...");
  delay(1000);
  Serial.begin(9600); 
}

#define MAXVALUE 1024

int val_hum = 879;
int val_brt = 667;
int val_temp = 37;

void loop()
{
  int chk = 0;
 
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
  
  delay(1000);
}

