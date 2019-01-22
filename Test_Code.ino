#include <TimedAction.h>

//this initializes a TimedAction class that will change the state of an LED every second.
TimedAction timedOn = TimedAction(10,blink);
TimedAction timedOff = TimedAction(100,blinkOff);
TimedAction rdgTimer = TimedAction(10,read);

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
timedOn.reset();
timedOn.enable();
timedOff.disable();
rdgTimer.disable();

pinMode(ledPin1,OUTPUT);
Serial.begin(9600);
digitalWrite(ledPin, LOW);
}

void loop(){
timedOn.check();
timedOff.check();
rdgTimer.check();
//Serial.println("Data Output:");
//Serial.println(analogRead(A0));
}

void blink(){
timedOn.disable();
timedOff.reset();
rdgTimer.reset();
timedOff.enable();
rdgTimer.enable();

if (count == 0){
  digitalWrite(ledPin,HIGH);
}
else if (count == 1){
  digitalWrite(ledPin1,HIGH);
}
else if (count == 2){
  digitalWrite(ledPin2,HIGH);
  }
count ++;
count = count%3;
}

void read(){
 incomingByte = analogRead(A0);
 Serial.println(incomingByte, DEC);
 rngTotal = rngTotal+incomingByte;
 done++;
 if(done < 10)
 {
   rdgTimer.reset();
 }
 else if (done == 10)
 {
   done = 0;
   avgPer = rngTotal/10;
   Serial.println("Average:");
   voltage = (avgPer / 1023.0) * 5.0;
   Serial.println(voltage, DEC);
   Serial.println();
   rngTotal = 0;
 }
 else {
     rdgTimer.disable();
 }
}


void blinkOff() {
timedOff.disable();
timedOn.reset();
timedOn.enable();
if (count == 0){
  digitalWrite(ledPin, LOW);
}
else if (count == 1){
  digitalWrite(ledPin1, LOW);
}
else if (count == 2){
  digitalWrite(ledPin2, LOW);
}

}
