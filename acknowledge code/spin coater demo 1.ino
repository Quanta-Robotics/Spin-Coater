
float RAMP1 = 6; 
int MAX1 = 6000; 
int SUS1 = 5; 

// second part of recipe
float RAMP2 = 5;
int MAX2 = 10000;
int SUS2 = 5;

// sets counters, pins and other variables used in calculations
int counter = 0;
int fanPulse = 0;
unsigned long pulseDuration;


void setup() {


  pinMode(3, OUTPUT); //set pin 3 as output for PWM
  Serial.begin(9600); //sets display output
  pinMode(fanPulse, INPUT); //0 pin for tachometer input
  digitalWrite(fanPulse,HIGH); //pull-up resistor for tachometer input
  pinMode(7, OUTPUT); // set pin 7 as gate for whole circuit


  TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM20); //sets prescalar values to modulate frequency and duty cycle
  TCCR2B = _BV(WGM22) | _BV(CS20); // sets ocr2a as measure
  OCR2A = 320; // register that determines max of 1 to 100% scale. Also determines frequency
  OCR2B = 0; // actual value that falls between 0 and OCR2A
  delay(3000);
   
}


void pwmDuty(byte ocrb) {
  OCR2B = ocrb;// setting a function that sets the obve OCR2B value at the end of each loop
}


float RPM2R(float RPM) // FLOATS, converts input RPM to INTERNAL SPEED SCALE
{
float result;
result = round((RPM - 2500)*(320/7500));
return result;
}


void loop() //main loop that uses an if loop and a counter to cycle through recipe steps. counter0 being first step and counter2 being termination of arduino
{
  if (counter == 0){
   
  for (int x1=0; x1 <= RPM2R(MAX1); x1++){
pwmDuty(x1);
delay((RAMP1*1000)/RPM2R(MAX1)); // delay between each iteration of ramp cycle that is a function of ramp time
pulseDuration = pulseIn(fanPulse, LOW); // initializes RPM output
int frequency = 1000000/pulseDuration;
Serial.print("RPM:");
Serial.println(frequency/4*60); // prints RPM
}
pwmDuty(RPM2R(MAX1));
for(int s1 = 0;s1 <= SUS1;s1++){
pulseDuration = pulseIn(fanPulse, LOW);
int frequency = 1000000/pulseDuration;
Serial.print("RPM:");
Serial.println(frequency/4*60);
delay(1000);
}
counter = 1; // sets counter to go to next part of if-loop
}
else if(counter == 1){
for (int x2; x2 < RPM2R(MAX2); x2++){
pwmDuty(x2);
pulseDuration = pulseIn(fanPulse, LOW);
int frequency = 1000000/pulseDuration;
Serial.print("RPM:");
Serial.println(frequency/4*60);
delay(RAMP2*1000/(RPM2R(MAX2)-RPM2R(MAX1)));
}
  pwmDuty(RPM2R(MAX2));
  for(int s1 = 0;s1 <= SUS1;s1++){
pulseDuration = pulseIn(fanPulse, LOW);
int frequency = 1000000/pulseDuration;
Serial.print("RPM:");
Serial.println(frequency/4*60);
delay(1000);
}
  counter = 2;
}


else if(counter == 2){
  pwmDuty(0); // set 0% duty cycle
  //set main transistor off
  Serial.print("Homing. Please wait");
while(1 != 0){
  pulseDuration = pulseIn(fanPulse, LOW);
int frequency = 1000000/pulseDuration;
Serial.print(".");
delay(1500);
if((frequency/4*60) == 0){break;}
}
Serial.print("Remove sample");
counter = 777;
}
}
