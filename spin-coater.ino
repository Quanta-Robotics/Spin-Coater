#include<Keypad.h> 
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>


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
Keypad keypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
Servo motor;


int value = 10;


typedef struct 
{
  int Speed;
  int Time;
  int Time_break;
}Stage;
Stage stage[10];

void start_slow(int val)
{
    val = map(val, 0, 7200, 30, 180);
    for(int i=0; i<val; i+=2)
    {
      motor.write(val);
      delay(100);
    }
}

void Run(int val)
{
  val = map(val, 0, 7200, 30, 180);
  motor.write(val);
}


int getNumber()
{
    char num[10];
    int i=0;
    while(1)
    {
        char key = keypad.waitForKey();
        if ('0' <= key && key <= '9')
        {
            num[i] = key;
            i++;
            lcd.print(key);        
        }
        else if( key == 'D')
        {
            num[i] =0;
            break;
        }    
    }
    return atoi(num);
}



void Show()
{
     lcd.print("Spin Coater");
     lcd.setCursor(0,1);
     lcd.print("Dept. of Chemistry");
     lcd.setCursor(0,2);
     lcd.print("Comilla University");
}



void takeInput(int i)
{
   lcd.clear();
   lcd.setCursor(4,1);
   lcd.print("Stage ");
   lcd.print(i);
   delay(1500);
   
   if(i>1)
   {
      lcd.clear();
      lcd.print("Enter time break in");
      lcd.setCursor(0,1);
      lcd.print("second between stage");
      lcd.setCursor(0,2);
      lcd.print(i-1);
      lcd.print(" and ");
      lcd.print(i);
      lcd.print(" : ");
      stage[i].Time_break = getNumber()*1000;
   }
   
    lcd.clear();
    lcd.print("Enter RPM ");
    lcd.setCursor(0,1);
    lcd.print("between(100 to 7000)");
    lcd.setCursor(0,2);
    lcd.print("for stage ");
    lcd.print(i);
    lcd.print(" : ");
    stage[i].Speed = getNumber();
    
    lcd.clear();
    lcd.print("Enter stage ");
    lcd.print(i);
    lcd.setCursor(0,1);
    lcd.print("running time : ");
    stage[i].Time = getNumber()*1000;
}



void Execute(int i)
{
  if(stage > 1)
  {
    lcd.clear();
    lcd.print("Break time before stage ");
    lcd.print(i);
    delay(stage[i].Time_break);
  }
  
  lcd.clear();
  lcd.print("Running stage ");
  lcd.print(i);
  start_slow(stage[i].Speed);
  Run(stage[i].Speed);
  delay(stage[i].Time);
  Run(0);
}



void Start()
{
    char start = keypad.waitForKey();
    if(start == 'D')
    {
        lcd.clear();
        lcd.print("How many stage you  want to run : ");
        int stg = getNumber(); 
        int val = stg;
        
        for(int i=1; i<=stg; i++)
        {
          takeInput(i);  
        }
        lcd.clear();
        lcd.print("Start processing...");
        delay(1000);

        for(int i=1; i<=stg; i++)
        {
          Execute(i);
        }
        lcd.clear();
        lcd.print("Stop Processing");
        delay(1000);
    } 
}

void maintener()
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Maintenance by:");
  lcd.setCursor(0,1);
  lcd.print("Dr. Md Abdul Majed");
  lcd.setCursor(0,2);
  lcd.print("Associate Professor");
  lcd.setCursor(0,3);
  lcd.print("Dept of Chemistry");
}

void developer()
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Developed by:");
  lcd.setCursor(0,1);
  lcd.print("Shanjit Mondol(Phy10");
  lcd.setCursor(0,2);
  lcd.print("Jewel Nath (ICT-13)");
  lcd.setCursor(0,3);
  lcd.print("Comilla University"); 
}
void setup()
{
     Serial.begin(9600);
          
     motor.attach(10,1000,2000);
     motor.write(0);
     
     lcd.init();
     lcd.backlight();
     
     keypad.addEventListener(keypadEvent);
}

void loop()
{
    lcd.clear();
    Show();
    
    
    Start();
    maintener();
    delay(5000);
    developer();
    while(1)
    {
      char key = keypad.waitForKey();
      if(key == 'D')
        break;
    }
}


void keypadEvent(KeypadEvent key)
{
    switch (keypad.getState())
    {
    case PRESSED:
        if (key == 'A') 
        {
           if(value < 180)
              value += 5;
           motor.write(value);        
        }
        if(key == 'B')
        {
           if(value > 0)
              value -= 5;
           motor.write(value);
        }
        if (key == 'C')
        {
            motor.write(0);
        }
        if (key == '#')
        {
            Start();
        }
        break;
    }
}