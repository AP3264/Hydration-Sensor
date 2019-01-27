
#include <TimedAction.h>
#include <math.h>

//this initializes a TimedAction class that will change the state of an LED every second.
TimedAction timedOn = TimedAction(10,blink); 
TimedAction timedOff = TimedAction(100,blinkOff);
TimedAction rdgTimer = TimedAction(4,read);

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
unsigned long start;
unsigned long end2;

void setup(){
  timedOn.reset();
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
  unsigned long stop1 = micros();
  //Serial.println("Time Difference");
  //Serial.print(stop1 - start);
  Serial.println("In blink");
  //Serial.println(time);
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
 start = micros();
 Serial.println("In read");
  //Serial.println(time);
 if(done < 4)
 {
  incomingByte = analogRead(A0);
  Serial.println(incomingByte, DEC);
  rngTotal = rngTotal+incomingByte;
  done++;
  rdgTimer.reset();
 }
 else if (done == 4)
 {
   end2 = micros();
   done = 0;
   //Serial.println("Taking measurements");
   incomingByte = analogRead(A0);
   Serial.println();
   Serial.print("Last measurement: ");
   //rdgTimer.disable();
   Serial.print(incomingByte, DEC);
   rngTotal = rngTotal+incomingByte;

   Serial.println();
   Serial.print("Last measurement - First Measurement: ");
   Serial.print(end2 - start);

//   Serial.println();
//   Serial.print("Start: ");
//   Serial.print(start);
//   Serial.println();
//   Serial.print("End2: ");
//   Serial.print(end2);
//   Serial.println();

   // Reset
   start = 0;
   end2=0;

   avgPer = rngTotal/5;
   Serial.println("Average:");
   voltage = (avgPer / 1023.0) * 5.0;
   Serial.println(voltage, DEC);
   Serial.println();
   rngTotal = 0;

   rdgTimer.disable();
 }
}


void blinkOff() {
  Serial.println("In blinkoff");
  //Serial.println(time);
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
