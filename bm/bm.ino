#include<Servo.h>

Servo bm; 
int val=0;

void setup()
{
  bm.attach(10,1000,2000);
  Serial.begin(9600);
}

void loop()
{ 
  if (Serial.available() > 0) {
   
    val = Serial.parseInt();
    val = map(val, 0, 1023, 0, 180);
    Serial.println(val);
  }
  bm.write(val);
  delay(5000);
}
