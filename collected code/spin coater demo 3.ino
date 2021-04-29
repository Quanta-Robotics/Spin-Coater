// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
Servo myesc;

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 500
#define ESC_PIN 13

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Key message
char msgs[5][15] = {"Right Key OK ",
                    "Up Key OK ",
                    "Down Key OK ",
                    "Left Key OK ",
                    "Select Key OK"
                   };
int adc_key_val[5] = {75, 250, 420, 620, 780 };
int adc_key_in;
int key = -1;
int oldkey = -1;
int speeds[] = {400, 500, 750, 1000, 1100, 1500, 2000, 3000, 4000, 5000, 7000};
int escvalues[] = {1093.9, 1106.1, 1136.7, 1167.2, 1179.4, 1228.2, 1289.3, 1411.4, 1533.5, 1655.6, 1899.8};
int times[] = {1, 1.5, 2, 5, 10, 20, 30, 45, 60, 120, 300, 600};
int speeds_size = sizeof(speeds) / sizeof(int) - 1;
int times_size = sizeof(times) / sizeof(int) - 1;
int selected_speed = 0;
int selected_time = 0;
String lyrics = "You spin me right round, wafer; Right round like a record, wafer; Right round round round; You spin me right round, wafer; Right round like a record, wafer; Right round round round               ";
int lyrics_size = sizeof(lyrics) / sizeof(char) - 1;


void setup() {
  Serial.begin(9600);
  myesc.attach(ESC_PIN);

  //myesc.writeMicroseconds(MAX_SIGNAL); //option to calibrate ESC
  //delay(4000);

  myesc.writeMicroseconds(MIN_SIGNAL);//start with motor off
  Serial.println("arming");

  lcd.begin(16, 2);// set up the LCD's number of columns and rows:
  lcd.setCursor(0, 0); lcd.print("Stefik Group 1.0");// Print a message to the LCD.
  lcd.setCursor(0, 1); lcd.print("you spin me"); delay(800);
  lcd.setCursor(0, 1); lcd.print("round like a   "); delay(800);
  lcd.setCursor(0, 1); lcd.print("record now     ");
  delay(800); //hold the load screen
  lcd.setCursor(0, 1); update_display();
}


void loop() {
  //lcd.setCursor(0, 1);// set the cursor to column 0, line 1 (note: line 1 is the second row, since counting begins with 0):
  adc_key_in = analogRead(0); // read the value from the sensor
  key = get_key(adc_key_in); // convert into key press

  if (key != oldkey) // if keypress is detected
  {

    delay(50); // wait for debounce time
    adc_key_in = analogRead(0); // read the value from the sensor
    key = get_key(adc_key_in); // convert into key press

    if (key != oldkey) //a new button press is detected!
    {
      oldkey = key; //avoid phantom presses durring continuous hold

      if (key == 0) //right key pressed
      {
        if (selected_time != times_size)
        {
          selected_time++;
        }
      }

      if (key == 1) //up key pressed
      {
        if (selected_speed != speeds_size)
        {
          selected_speed++;
        }
      }

      if (key == 2) //down key pressed
      {
        if (selected_speed > 0)
        {
          selected_speed--;
        }
      }

      if (key == 3) //left key pressed
      {
        if (selected_time > 0)
        {
          selected_time--;
        }
      }

      if (key == 4) //select key pressed
      {
        //take 1 second to spin up
        int mystep;
        int numberofsteps = 10;
        int nextspeed;
        for (mystep = 0; mystep < numberofsteps; mystep++) //take 100 incraments from slowest speed to requested speed
        {
          nextspeed = escvalues[0] + (escvalues[selected_speed] - escvalues[0]) * mystep / numberofsteps;
          myesc.writeMicroseconds(nextspeed);//set the motor speed
          Serial.println(nextspeed);
          delay(1000 / numberofsteps);
        }

        myesc.writeMicroseconds(escvalues[selected_speed]);
          Serial.println("requested speed");

        
        int t; int cursorpos;
        for (t = times[selected_time] - 1; t >= 0; t--) //continue spinning for [requested time minus spinup time]
        {
          cursorpos = (times[selected_time] - t) * 2;
          lcd.setCursor(0, 0); lcd.print(lyrics.substring(cursorpos, cursorpos + 16));
          delay(500);
          cursorpos++;
          lcd.setCursor(0, 0); lcd.print(lyrics.substring(cursorpos, cursorpos + 16));
          lcd.setCursor(0, 1); lcd.print("woosh!        ");
          lcd.setCursor(9, 1); lcd.print(t); lcd.print("s");
          delay(500);
        }
        myesc.writeMicroseconds(MIN_SIGNAL);//turn off motor
      }


      update_display();

    } //
  } //new key pressed
} //overall loop

void update_display() {
  lcd.setCursor(0, 0); lcd.print("Stefik Group 1.0");// Print a message to the LCD.
  lcd.setCursor(0, 1);
  lcd.print(speeds[selected_speed]);
  lcd.print(" RPM, ");
  lcd.print(times[selected_time]);
  lcd.print("s        " );
}

int get_key(unsigned int input) // Convert ADC value to key number
{ int k;
  for (k = 0; k < 5; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }
  if (k >= 5)
    k = -1; // No valid key pressed
  return k;
}