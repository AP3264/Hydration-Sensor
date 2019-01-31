
#include <TimedAction.h>
#include <math.h>

//this initializes a TimedAction class that will change the state of an LED every second.
TimedAction timedOn = TimedAction(10,blink); 
TimedAction timedOff = TimedAction(100,blinkOff);
TimedAction rdgTimer = TimedAction(1,read);
TimedAction listTimer = TimedAction(330, average);
TimedAction fullCycle = TimedAction(500, cycle);

#define NUM_MEASUREMENTS 50

int list1[NUM_MEASUREMENTS]; // array of # of measurements
int list2[NUM_MEASUREMENTS];
int list3[NUM_MEASUREMENTS];

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
int i;

void setup(){
  timedOn.reset();
  timedOn.enable();
  listTimer.enable();
  fullCycle.enable();
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
  listTimer.check();
  fullCycle.check();
}

void blink(){
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
 if(done < NUM_MEASUREMENTS)
 {
  incomingByte = analogRead(A0);
  // insert measurement to the list
  for(i=0; i<NUM_MEASUREMENTS; i++) {
    if (count == 0) { // LED = 670
        list1[i] = incomingByte;
    }
     if (count == 1) { // LED = 850
        list2[i] = incomingByte;
    }
     if (count == 2) { // LED = 950
        list3[i] = incomingByte;
    }
  }
  //Serial.println(incomingByte, DEC);
  rngTotal = rngTotal+incomingByte;
  done++;
  rdgTimer.reset();
 }
 else if (done == NUM_MEASUREMENTS)
 {
   done = 0;
   
   // insert last measurement
   incomingByte = analogRead(A0);
   if (count == 0) { // LED = 540
      list1[NUM_MEASUREMENTS-1] = incomingByte;
    }
   if (count == 1) { // LED = 650
      list2[NUM_MEASUREMENTS-1] = incomingByte;
    }
   if (count == 2) { // LED = 750
      list3[NUM_MEASUREMENTS-1] = incomingByte;
    }
  
   //Serial.print(incomingByte, DEC);
   rngTotal = rngTotal+incomingByte;


//// Averaging
//   avgPer = rngTotal/5;
////   Serial.println("Average:");
//   voltage = (avgPer / 1023.0) * 5.0;
////   Serial.println(voltage, DEC);
//   rngTotal = 0;

   rdgTimer.disable();
 }
}

void average() {
  rdgTimer.disable();
  timedOn.disable();
  timedOff.disable();
  Serial.println("List 1: ");
  for(i=0;i<NUM_MEASUREMENTS;i++)
  {
    
    Serial.println(list1[i]);
  }
    Serial.println("List 2: ");
    for(i=0;i<NUM_MEASUREMENTS;i++)
  {

    Serial.println(list2[i]);
  }
    Serial.println("List 3: ");
    for(i=0;i<NUM_MEASUREMENTS;i++)
  {
    
    Serial.println(list3[i]);
  }

   


}

//enable timers in the 5th timer
void cycle() {
  timedOff.enable();
  timedOn.enable();
  rdgTimer.enable();
  listTimer.enable();

  // clear the lists 

    memset(list1, NULL, sizeof(list1));
    memset(list2, NULL, sizeof(list2));
    memset(list3, NULL, sizeof(list3));
}

void blinkOff() {
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
