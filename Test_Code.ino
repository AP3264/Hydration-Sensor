
#include <TimedAction.h>
#include <math.h>

//this initializes a TimedAction class that will change the state of an LED every second.
TimedAction timedOn = TimedAction(10,blink);
TimedAction timedOff = TimedAction(100,blinkOff);
TimedAction rdgTimer = TimedAction(6,read);

unsigned long time;

//pin / state variables
#define ledPin 13
#define ledPin1 12
#define ledPin2 11

boolean ledState = false;
int incomingByte = 0;
int rngTotal = 0;
int avgPer = 0;
int done = 0;
int count = 0;
float voltage;


void setup(){
//timedOn.reset();
timedOn.enable();
timedOff.disable();
rdgTimer.disable();

pinMode(ledPin1,OUTPUT);
pinMode(ledPin,OUTPUT);
pinMode(ledPin2,OUTPUT);
Serial.begin(9600);
digitalWrite(ledPin, HIGH);
digitalWrite(ledPin1, HIGH);
digitalWrite(ledPin2, HIGH);
}

void loop(){
timedOn.check();
timedOff.check();
rdgTimer.check();
//Serial.println("Data Output:");
//Serial.println(analogRead(A0));
time = millis();
}

void blink(){
Serial.println("In blink");
Serial.println(time);
timedOn.disable();
timedOff.reset();
rdgTimer.reset();
timedOff.enable();
rdgTimer.enable();
if (count == 0){
  digitalWrite(ledPin,LOW);
}
else if (count == 1){
  digitalWrite(ledPin1,LOW);
}
else if (count == 2){
  digitalWrite(ledPin2,LOW);
  }
}

void read(){
  Serial.println("In read");
  Serial.println(time);
 if(done < 15)
 {
  incomingByte = analogRead(A0);
  Serial.println(incomingByte, DEC);
  rngTotal = rngTotal+incomingByte;
  done++;
  rdgTimer.reset();
 }
 else if (done == 15)
 {
   done = 0;
   //Serial.println("Taking measurements");
   incomingByte = analogRead(A0);
   Serial.println("Last measurement");
   Serial.println(incomingByte, DEC);
   rngTotal = rngTotal+incomingByte;
//   avgPer = rngTotal/15;
//   Serial.println("Average:");
//   Serial.println(count);
//   voltage = (avgPer / 1023.0) * 5.0;
//   Serial.println(voltage, DEC);
//   Serial.println();
//   rngTotal = 0;

     rdgTimer.disable();
 }
}


void blinkOff() {
Serial.println("In blinkoff");
Serial.println(time);
timedOff.disable();
timedOn.reset();
timedOn.enable();

if (count == 0){
  digitalWrite(ledPin, HIGH);
}
else if (count == 1){
  digitalWrite(ledPin1, HIGH);
}
else if (count == 2){
  digitalWrite(ledPin2, HIGH);
}
count ++;
count = count%3;
}
