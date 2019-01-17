#include <TimedAction.h>

//this initializes a TimedAction class that will change the state of an LED every second.
TimedAction timedOn = TimedAction(10,blink);
TimedAction timedOff = TimedAction(100,blinkOff);
TimedAction rdgTimer = TimedAction(8,read);

//pin / state variables
#define ledPin 13
boolean ledState = false;
int incomingByte = 0;
int rngTotal = 0;
int avgPer = 0;
int done = 0;


void setup(){
timedOn.enable();
timedOff.disable();
rdgTimer.disable();

pinMode(ledPin,OUTPUT);
Serial.begin(9600);
digitalWrite(ledPin,HIGH);
}

void loop(){
timedOn.check();
timedOff.check();
rdgTimer.check();

}

void blink(){
timedOn.disable();
timedOff.enable();
rdgTimer.enable();
digitalWrite(ledPin,HIGH);

int i = 0;
}

void read(){
 incomingByte = analogRead(A0);
 Serial.println(incomingByte, DEC);
 rngTotal = rngTotal+incomingByte;
 i++;
 if(i < 10)
 {
   rdgTimer.reset();
 }
 else
 {
   rdgTimer.disable();
   avgPer = rngTotal/10;
   Serial.println(avgPer, DEC);
 }
}


void blinkOff() {
timedOff.disable();
timedOn.enable();
digitalWrite(ledPin, LOW);
}
