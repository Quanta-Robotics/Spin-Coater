#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  lcd.init();
  lcd.backlight();
}

void loop()
{
  int i=5;
  lcd.clear();
  lcd.print("Enter stage ");
  lcd.print(i);
  lcd.setCursor(0,1);
  lcd.print("running time : ");
  
  
  delay(500);
}
