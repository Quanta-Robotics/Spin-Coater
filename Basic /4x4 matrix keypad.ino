#include<Keypad.h> 
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte numRows= 4;
const byte numCols= 4;
                   

char keymap[numRows][numCols]=
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {9,8,7,6};
byte colPins[numCols]= {5,4,3,2};


Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
     Serial.begin(9600);
     lcd.init();
     
     lcd.backlight();
     lcd.home ();
     lcd.print("Press a key");
     delay(2000);
}

void loop()
{
    char keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY)
    {
      lcd.clear();
      lcd.print(keypressed);
    }
}
