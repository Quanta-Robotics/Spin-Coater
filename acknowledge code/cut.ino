#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 2);

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'C', '0', '=', '/'}};
byte rowPins[ROWS] = {10, 9, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

boolean presentValue = false;
boolean next = false;
boolean final = false;
String num1, num2;
int answer = 0;
char op;

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(3, 0);
    lcd.print("Maker UNO");
    lcd.setCursor(3, 1);
    lcd.print("Calculator");
    delay(3000);
    lcd.clear();
}

void loop()
{
    char key = myKeypad.getKey();

    if (key != NO_KEY && (key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0'))
    {
        if (presentValue != true)
        {
            num1 = num1 + key;
            int numLength = num1.length();
            lcd.setCursor(0, 0);
            lcd.print(num1);
        }
        else
        {
            num2 = num2 + key;
            int numLength = num2.length();
            int numLength1 = num1.length();
            lcd.setCursor(1 + numLength1, 0);
            lcd.print(num2);
            final = true;
        }
    }

    else if (presentValue == false && key != NO_KEY && (key == '/' || key == '*' || key == '-' || key == '+'))
    {
        if (presentValue == false)
        {
            int numLength = num1.length();
            presentValue = true;
            op = key;
            lcd.setCursor(0 + numLength, 0);
            lcd.print(op);
        }
    }

    else if (final == true && key != NO_KEY && key == '=')
    {

        if (op == '+')
        {
            answer = num1.toInt() + num2.toInt();
        }
        else if (op == '-')
        {
            answer = num1.toInt() - num2.toInt();
        }
        else if (op == '*')
        {
            answer = num1.toInt() * num2.toInt();
        }
        else if (op == '/')
        {
            answer = num1.toInt() / num2.toInt();
        }
        lcd.clear();
        lcd.setCursor(16, 1);
        lcd.autoscroll();
        lcd.print(answer);
        lcd.noAutoscroll();
    }
    else if (key != NO_KEY && key == 'C')
    {
        lcd.clear();
        presentValue = false;
        final = false;
        num1 = "";
        num2 = "";
        answer = 0;
        op = ' ';
    }
}
