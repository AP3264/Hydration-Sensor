//Hydration Sensor
//Drexel University - Senior Design Team 18
//Anav Pradhan, Anishi Patel, Tyler Harris, Nicholas Joseph, Anishi Patel

#include <LinkedList.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

#include <SPI.h>
byte address = 0x11;
int CS= 4;
int i=0;
int driverPin = 3;

//Initialize required variables
LinkedList<int> valList670;
LinkedList<int> valList850;
LinkedList<int> valList950;
LinkedList<unsigned long> time670;
LinkedList<unsigned long> time850;
LinkedList<unsigned long> time950;
unsigned long Start;
unsigned short Count;
unsigned short Count2;
unsigned short Count3;
unsigned short Tracker;
unsigned long timer;
int result;

//Setup ADC
Adafruit_ADS1115 adc;
int adc0;
int adc1;
int adc3;

#define ledPin670 7
#define ledPin850 8
#define ledPin950 9

//readSPeed = rate at which individual samples are taken
#define readSpeed 5

//Cycletime = Time for which each LED is on, PauseTime = Interval time while switching LEDs, TotalTime = Time of entire cycle
#define CycleTime 100
#define PauseTime 10
#define TotalTime 500

#define mux1 10 //S0
#define mux2 11 //S1
#define mux3 12 //S2
#define enablePin 13


void setup( void )
{

  Serial.begin( 38400 );
  while(!Serial){}
  Wire.begin();
  adc.begin();

//  while(!Serial.available());
//  result = Serial.read();
  
  
//  pinMode (ledPin670, OUTPUT);
//  pinMode (ledPin850, OUTPUT);
//  pinMode (ledPin950, OUTPUT);
  pinMode(mux1, OUTPUT);
  pinMode(mux2, OUTPUT);
  pinMode(mux3, OUTPUT);
  pinMode(enablePin, OUTPUT);  

  pinMode (CS, OUTPUT);
  pinMode (driverPin, OUTPUT);
  digitalWrite(driverPin, HIGH);
  
  digitalWrite(ledPin670,HIGH);
  digitalWrite(ledPin850,HIGH);
  digitalWrite(ledPin950,HIGH);
  digitalWrite(enablePin, LOW);
  
  Start = millis();
  timer = micros();
  
  SPI.begin();
// // adjust high and low resistance of potentiometer
// // adjust Highest Resistance .
//  digitalPotWrite(0x00);
//  
//
//     // adjust  wiper in the  Mid point  .
//  digitalPotWrite(0x80);
//
//  // adjust Lowest Resistance .
//  digitalPotWrite(0xFF);
}

int digitalPotWrite(int value)
{
   digitalWrite(CS, LOW);
   SPI.transfer(address);
   SPI.transfer(value);
   digitalWrite(CS, HIGH);
}

//Initializes and sends potentiometer value
void set_pot_val(int byteVal)
{
    Wire.beginTransmission(44);
    Wire.write(byte(0x00));
    Wire.write(byte(byteVal));
    Wire.endTransmission();
}

//Adds current time and OPT101 reading to specified lists
void add_to_list(LinkedList<int> &pinNum,LinkedList<unsigned long> &timeList, int analogPin)
{
   // pinNum.add(analogRead( analogIn ));
    pinNum.add(adc.readADC_SingleEnded(analogPin));
    timeList.add(micros() - timer);
}

//Print output to serial
void print_to_ser(LinkedList<int> &pinNum,LinkedList<unsigned long> &timer, int valDiff)
{
//This function takes in the list of time and reading values, formats it and prints to serial

  int listSize = pinNum.size();
  for (int h = 0; h < listSize; h+=valDiff) {
      //Get value from list
      if (h <= listSize){
        int val = pinNum.get(h);
        unsigned long tim = timer.get(h);
        Serial.print(tim);
        Serial.print("\t");
        //Serial.println(val);
        Serial.println((val/32768.0)*6144);
      }
  }
  if ((Tracker%3)==1){
    Serial.println("L1");
    Serial.println("L1");
  }
  else if ((Tracker%3)==2){
    Serial.println("L2");
  }
  else{
    Serial.println("L3");
  }
 
}

void loop( void )
{
  //For first LED
  if ( millis() - Start < CycleTime )
  {
    if (Count == 0)
    {
//      Serial.println("L1");
    }

    digitalWrite(ledPin670,LOW);
    digitalWrite(ledPin850, HIGH);
    digitalWrite(ledPin950, HIGH);
    
//    digitalWrite(driverPin, LOW);
//    delay(5);
    digitalPotWrite(122);
//    delay(5);
//    digitalWrite(driverPin, HIGH);
    delay(readSpeed);

    // channel A0 ON
    digitalWrite(mux1,LOW); //S0
    digitalWrite(mux2,LOW); //S1
    digitalWrite(mux3,LOW); //S2
    digitalWrite(enablePin, LOW);

    add_to_list(valList670, time670,0);
    ++Count;
    
    // Turn Channel A0 off
    digitalWrite(enablePin, HIGH);
  }

  //First interval between LED switch
  else if ((millis() - Start >= CycleTime) && (millis() - Start < (CycleTime + PauseTime)))
  {
    // Turn off 670
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850, HIGH);
    digitalWrite(ledPin950, HIGH);
    
  }

  //For second LED  
  else if ((millis() - Start >= (CycleTime+PauseTime)) && (millis() - Start < (2*CycleTime+PauseTime)))
  {
    if (Count2 == 0){
//    Serial.println("L2");
    }

    // turn 850 on
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,LOW);
    digitalWrite(ledPin950, HIGH);
    
    digitalPotWrite(131);
    
    delay(readSpeed);

    // Channel A1 ON
    digitalWrite(mux1,HIGH); // S0
    digitalWrite(mux2,LOW);  // S1
    digitalWrite(mux3,LOW);  // S2
    digitalWrite(enablePin, LOW);
    
    add_to_list(valList850, time850,1);
    ++Count2;

    // Turn Channel A1 off
    digitalWrite(enablePin, HIGH);
  }

  //interval between LED switch
  else if ((millis() - Start) >= ((2*CycleTime)+(PauseTime)) && (millis() - Start) < ((2*CycleTime)+(2*PauseTime)))
  {
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin670, HIGH);
    digitalWrite(ledPin950, HIGH);
    
  }
  
  //For third LED  
  else if ((millis() - Start >= ((2*CycleTime)+(2*PauseTime))) && (millis() - Start < ((3*CycleTime)+(2*PauseTime))))
  {
    if (Count3 == 0)
    {
//    Serial.println("L3");
    }

    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,LOW);
    digitalWrite(ledPin670, HIGH);
    
    digitalPotWrite(137);
    delay(readSpeed);
    
    // Channel A3 ON
    digitalWrite(mux1,LOW);  //S0
    digitalWrite(mux2,HIGH); //S1
    digitalWrite(mux3,LOW);  //S2
    digitalWrite(enablePin, LOW);

    add_to_list(valList950, time950,3);
    ++Count3;

    // Turn Channel A2 off
    digitalWrite(enablePin,HIGH);
  }

  //Process collected data, reset all values, reset timer
  else
  {
    digitalWrite(ledPin950,HIGH);
    digitalWrite(ledPin850, HIGH);
    digitalWrite(ledPin670, HIGH);
    
   
    //Print all values to serial port
    ++Tracker; 
    print_to_ser(valList670, time670,1);
    ++Tracker;
    print_to_ser(valList850, time850,1);
    ++Tracker;
    print_to_ser(valList950, time950,1);

    //Reset all list to empty
    valList670.clear();
    valList850.clear();
    valList950.clear();
    time850.clear();
    time670.clear();
    time950.clear();
    
    //Reset Counters to 0 and restart timer
    while ((millis()-Start)<=TotalTime){     
      if ((millis()-Start)>TotalTime){
        break;
        }
    }
    
    Count = 0;
    Count2 = 0;
    Count3 = 0;
    Start = millis();

    //Ensure all LEDs switches off
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,HIGH);
  //  digitalWrite(enablePin,HIGH);
    }
    
}
