//Hydration Sensor
//Drexel University - Senior Design Team 18
//Anav Pradhan, Anishi Patel, Tyler Harris, Nicholas Joseph, Anishi Patel

#include <LinkedList.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

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
Adafruit_ADS1015 adc;
int adc0;
int adc1;
int adc3;

#define ledPin670 6
#define ledPin850 5
#define ledPin950 4

//readSPeed = rate at which individual samples are taken
#define readSpeed 5

//Cycletime = Time for which each LED is on, PauseTime = Interval time while switching LEDs, TotalTime = Time of entire cycle
#define CycleTime 100
#define PauseTime 10
#define TotalTime 500

#define mosfetPin 9
#define mosfetPin_2 8
#define mosfetPin_3 7


void setup( void )
{
  Serial.begin( 38400 );
  while(!Serial){}
  Wire.begin();
  adc.begin();

//  while(!Serial.available());
//  result = Serial.read();
  
  
  pinMode (ledPin670, OUTPUT);
  pinMode (ledPin850, OUTPUT);
  pinMode (ledPin950, OUTPUT);
  pinMode(mosfetPin, OUTPUT);
  pinMode(mosfetPin_2, OUTPUT);
  pinMode(mosfetPin_3, OUTPUT);
  
  digitalWrite(ledPin670,HIGH);
  digitalWrite(ledPin850,HIGH);
  digitalWrite(ledPin950,HIGH);
  
  Start = millis();
  timer = micros();
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
        Serial.println((val/2048.0)*6144);
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
    set_pot_val(21);
    digitalWrite(mosfetPin,HIGH);

    if (Count == 0)
    {
//      Serial.println("L1");
    }

    delay(readSpeed);
    digitalWrite(ledPin670,LOW);
    adc.readADC_SingleEnded(0);
    add_to_list(valList670, time670,0);
    ++Count;
  }

  //First interval between LED switch
  else if ((millis() - Start >= CycleTime) && (millis() - Start < (CycleTime + PauseTime)))
  {
    digitalWrite(ledPin670,HIGH);
    digitalWrite(mosfetPin,LOW);
  }

  //For second LED  
  else if ((millis() - Start >= (CycleTime+PauseTime)) && (millis() - Start < (2*CycleTime+PauseTime)))
  {
    set_pot_val(27);
    digitalWrite(mosfetPin_2,HIGH);

    if (Count2 == 0){
//    Serial.println("L2");
    }
    
    delay(readSpeed);
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,LOW);
    adc.readADC_SingleEnded(1);
    
    add_to_list(valList850, time850,1);

    ++Count2;
  }

  //interval between LED switch
  else if ((millis() - Start) >= ((2*CycleTime)+(PauseTime)) && (millis() - Start) < ((2*CycleTime)+(2*PauseTime)))
  {
    digitalWrite(ledPin850,HIGH);
    digitalWrite(mosfetPin_2,LOW);
  }
  
  //For third LED  
  else if ((millis() - Start >= ((2*CycleTime)+(2*PauseTime))) && (millis() - Start < ((3*CycleTime)+(2*PauseTime))))
  {
    set_pot_val(29);
    digitalWrite(mosfetPin_3,HIGH);
    
    if (Count3 == 0)
    {
//    Serial.println("L3");
    }
    
    delay(readSpeed);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,LOW);
    adc.readADC_SingleEnded(3);
    
    add_to_list(valList950, time950,3);
    ++Count3;
  }

  //Process collected data, reset all values, reset timer
  else
  {
    digitalWrite(ledPin950,HIGH);
    digitalWrite(mosfetPin_3,LOW);
    
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

    //Ensure all LEDs off
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,HIGH);
    }
    
}
