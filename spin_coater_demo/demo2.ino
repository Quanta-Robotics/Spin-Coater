  #include <Keypad.h>
    #include <Wire.h>  // Comes with Arduino IDE
    #include <LiquidCrystal_I2C.h>
    #include <Servo.h>
    Servo myservo;
    LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

    const byte ROWS = 4; //four rows
    const byte COLS = 3; //three columns
    char keys[ROWS][COLS] =
    {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
    };

    byte rowPins[ROWS] = {9,8,7,6}; //row pinouts of the keypad (L1, L2, L3, L4)
    byte colPins[COLS] = {5,4,3}; //column pinouts of the keypad (R1, R2, R3)
    Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

    // STATE CONDITION FOR MAIN LOOP
    enum { enter_values, spin , finish } systemstate;
    unsigned long previousMillis = 0;
    

   // RPM MEASUREMENT
   const int dataIN = 2; //IR sensor INPUT
   unsigned long prevmillis; // To store time
   unsigned long duration; // To store time difference
   unsigned long lcdrefresh; // To store time for lcd to refresh
   int rpm; // RPM value
   boolean currentstate; // Current state of IR input scan
   boolean prevstate; // State of IR sensor in previous scan

    // DECLARE
   int stage1speed , stage1time ,stage2speed , stage2time ,stage3speed , stage3time ;

    void setup()
    { 
    Serial.begin(9600);
    lcd.begin(20,4);
    myservo.attach(11);
  
    systemstate = enter_values;  // set up the starting state

    pinMode(dataIN,INPUT);   
    prevmillis = 0;
    prevstate = LOW;  
   
    lcd.setCursor(4,1);
    lcd.print("SPIN COATER");
    lcd.setCursor(6,2);
    lcd.print("MACHINE");
    delay(5000);
    lcd.clear();

    lcd.setCursor(3,1);
    lcd.print("S = speed(sv)");
    lcd.setCursor(3,2);
    lcd.print("T = time(sec)");
    delay(5000);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("S=");
    lcd.setCursor(0,1);
    lcd.print("T=");
    lcd.setCursor(0,2);
    lcd.print("S=");
    lcd.setCursor(0,3);
    lcd.print("T=");
    lcd.setCursor(10,0);
    lcd.print("S=");
    lcd.setCursor(10,1);
    lcd.print("T=");
    }

    //FUNCTION FOR KEY IN SPEED AND TIME
    void enter_speed_time()
    {
  
     stage1speed = getTheNumber();
     lcd.setCursor(2,0);
     lcd.print(stage1speed);
     lcd.print("sv");
     stage1time = getTheNumber();
     lcd.setCursor(2,1);
     lcd.print(stage1time);
     lcd.print("sec");
  
     stage2speed = getTheNumber();
     lcd.setCursor(2,2);
     lcd.print(stage2speed);
     lcd.print("sv");
     stage2time = getTheNumber();
     lcd.setCursor(2,3);
     lcd.print(stage2time);
     lcd.print("sec");
  

     stage3speed = getTheNumber();
     lcd.setCursor(12,0);
     lcd.print(stage3speed);
     lcd.print("sv");
     stage3time = getTheNumber();
     lcd.setCursor(12,1);
     lcd.print(stage3time);
     lcd.print("sec");
    }

   // FUNCTION TO GET NUMBERS FROM 4X3 MATRIC KEYPAD
   int getTheNumber()
   {
    char buffer[4];
    int i=0;
    while (1)
    {
        char key = keypad.getKey();

        // If it's a number AND we have space left, add to our string
        if ('0' <= key && key <= '9' && i < 3)
        {
            buffer[i] = key;
            i++;        
        }
        // If it's a * or #, end
        else if ('#' == key && i > 0)
        {
            // Null terminate
            buffer[i] =0; 
            int value = atoi(buffer);  // Convert to an integer
            break;
        }    
    }
    return atoi(buffer);
    }

    // FUNCTION FOR RPM MEASUREMENT
     void rpmMeasure()
     {
     // RPM Measurement
     currentstate = digitalRead(dataIN); // Read IR sensor state
     if( prevstate != currentstate) // If there is change in input
     {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         duration = ( micros() - prevmillis ); // Time difference between revolution in microsecond
         rpm = (60000000/duration); // rpm = (1/ time millis)*1000*1000*60;
         prevmillis = micros(); // store time for next revolution calculation
       }
      }
     prevstate = currentstate; // store this scan (prev scan) data for next scan
      lcd.setCursor(10,3);
      lcd.print(rpm);
      lcd.print("rpm");         
      }
  

     // FUNCTION FOR MOTOR SPINNING ACCORDING TO TIME AND SPEED   
    void motorspin()
    {  
   
     unsigned long currentMillis = millis();
     int idleValue = 0;

     static enum { IDLE, STAGE1, STAGE2, STAGE3 } spinningstate;
     switch (spinningstate) {
      case IDLE:
            myservo.write(stage1speed);
            previousMillis =currentMillis; 
            spinningstate = STAGE1;
            
        break;
    case STAGE1:
        if (currentMillis - previousMillis >= stage1time*1000) {
            myservo.write(stage2speed);
            previousMillis = currentMillis;
            spinningstate = STAGE2;
        }
        break;
    case STAGE2:
        if (currentMillis - previousMillis >= stage2time*1000) {
            myservo.write(stage3speed);
            previousMillis = currentMillis;
            spinningstate = STAGE3;
            
        }
        break;
    case STAGE3:
        if (currentMillis - previousMillis >= stage3time*1000) {
            myservo.write(idleValue);
            spinningstate = IDLE;
        }
        break;
      }
      }

      // MAIN LOOP
      void loop()
      { 
       switch(systemstate)
      {  
    case enter_values:
    
        enter_speed_time();
        systemstate = spin;
        break;
        
    case spin: 
    
       rpmMeasure();
       motorspin();
       if (if enough time have passed ) // haven't figure out yet
       {
        systemstate = finish;
       }
       break;
       
    case finish:
    
       systemstate = enter_values;
       break;        
    }
    }
  