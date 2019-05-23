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

int digPot670;
int digPot850;
int digPot950;

//Setup ADC
Adafruit_ADS1115 adc;
int adc0;
int adc1;
int adc3;

// Mutliplexer Pins to Switch the LEDs
#define ledPin670 7 //s0
#define ledPin850 8 // s1
#define ledPin950 9 // s2
#define enablePin_LED 6 

//readSPeed = rate at which individual samples are taken
#define readSpeed 5

//Cycletime = Time for which each LED is on, PauseTime = Interval time while switching LEDs, TotalTime = Time of entire cycle
#define CycleTime 100
#define PauseTime 10
#define TotalTime 500

// Multiplexer Pins for pre-filtered signals
#define mux1 12 //S0
#define mux2 11 //S1
#define mux3 10 //S2
#define enablePin 13

// Gain Multiplexer
#define enablePin_gain 5 
#define s0 0 //S0 channel 0
#define s1 1 //S1 channel 1  
#define s2 2 //S2 channel 2 

int data;
int pot_data;

void setup( void )
{

  Serial.begin( 38400 );
  while(!Serial){}
  Wire.begin();
  adc.begin();

  pinMode (ledPin670, OUTPUT);
  pinMode (ledPin850, OUTPUT);
  pinMode (ledPin950, OUTPUT);
  pinMode (enablePin_LED, OUTPUT);
  
  pinMode(mux1, OUTPUT);
  pinMode(mux2, OUTPUT);
  pinMode(mux3, OUTPUT);
  pinMode(enablePin, OUTPUT);  

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(enablePin_gain, OUTPUT);  

  pinMode (CS, OUTPUT);
  pinMode (driverPin, OUTPUT);
  digitalWrite(driverPin, HIGH);
  
  digitalWrite(ledPin670,LOW);
  digitalWrite(ledPin850,LOW);
  digitalWrite(ledPin950,LOW);
  digitalWrite(enablePin_LED, LOW);
  
  digitalWrite(enablePin, HIGH);
  
  digitalWrite(enablePin_gain, LOW);

//  digitalWrite(mux1,LOW);
//  digitalWrite(mux2,LOW);
//  digitalWrite(mux3,LOW);

  while(!Serial.available());
  {
    Serial.println("waiting for gain");

  }

  while(Serial.available()) {
    // Gain Multiplexer
    data = Serial.read();  // data is gain value: 1,5,10,15,20
    if (data == '1')
    {
      // Channel A0 = gain 1
      digitalWrite(s2, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s0, LOW);
      digitalWrite(enablePin_gain, LOW);
    }

    else if (data == '2')
    {
       // Channel A1 = gain 5
      digitalWrite(s2, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s0, HIGH);
      digitalWrite(enablePin_gain, LOW);
    }

    else if (data == '3')
    {
      // Channel A2 = gain 10
      digitalWrite(s2, LOW);
      digitalWrite(s1, HIGH);
      digitalWrite(s0, LOW);
      digitalWrite(enablePin_gain, LOW);
    }

    else if (data == '4')
    {
      // Channel A4 = gain 15
      digitalWrite(s2, HIGH);
      digitalWrite(s1, LOW);
      digitalWrite(s0, LOW);
      digitalWrite(enablePin_gain, LOW);
    }

    else if (data == '5')
    {
      // Channel A6 = gain 20
      digitalWrite(s2, HIGH);
      digitalWrite(s1, HIGH);
      digitalWrite(s0, LOW);
      digitalWrite(enablePin_gain, LOW);
    }
  }

//   while(!Serial.available());
//  {
//    Serial.println("waiting for gain");
//
//  }

//  while(Serial.available()) {
//    pot_data = Serial.read(); 
//
//    if (pot_data == '1')
//    {
//    // current 10
//      digPot670 = 217;
//      Serial.println(digPot670);
//      digPot850 = 232;
//      digPot950 = 244;
//    }
//
//    else if (pot_data == '2')
//    {
//      // current 15
//      digPot670 = 122;
//      digPot850 = 131;
//      digPot950 = 137;
//    }
//
//    else if (pot_data == '3')
//    {
//      // current 20
//      digPot670 = 76;
//      digPot850 = 82;
//      digPot950 = 87;
//    }
//
//    else if (pot_data == '4')
//    {
//      // current 25
//      digPot670 = 50;
//      digPot850 = 55;
//      digPot950 = 58;
//    }
//
//    else if (pot_data == '5')
//    {
//      // current 30
//      digPot670 = 33;
//      digPot850 = 37;
//      digPot950 = 39;
//    }
//
//    else if (pot_data == '6')
//    {
//      // current 35
//      digPot670 = 21;
//      digPot850 = 25;
//      digPot950 = 26;
//    }
//  }
//      digPot670 = 217;
//      digPot850 = 232;
//      digPot950 = 244;
  
  
  Start = millis();
  timer = micros();
  
  SPI.begin();
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
    // routing the signal to the filter (pre-filtered signal)
    // channel A0 ON

    digitalWrite(enablePin, HIGH);
    digitalWrite(ledPin670,LOW);
    digitalWrite(ledPin850, HIGH);
    digitalWrite(ledPin950, HIGH);
    
    digitalWrite(mux1,LOW); //S0
    digitalWrite(mux2,LOW); //S1
    digitalWrite(mux3,LOW); //S2
    digitalWrite(enablePin, LOW);
//    delay(5);
    }



    // Logic for multiplexer switching the LEDS (670nm)
    // Channel A0 ON
//    digitalWrite(driverPin, HIGH);

//    digitalWrite(enablePin_LED, LOW);
    
//    digitalWrite(driverPin, LOW);
//    delay(5);
//    delay(5);
//    digitalWrite(driverPin, HIGH);
    delay(readSpeed);    


    add_to_list(valList670, time670,0);
    ++Count;
    
    // Turn Channel A0 off for LED mux
    
    
  }

  //First interval between LED switch
  else if ((millis() - Start >= CycleTime) && (millis() - Start < (CycleTime + PauseTime)))
  {
    // Turn off 670 (switches)
//    digitalWrite(enablePin_LED, HIGH);
    digitalWrite(enablePin,HIGH);
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950, HIGH);


  }

  //For second LED  
  else if ((millis() - Start >= (CycleTime+PauseTime)) && (millis() - Start < (2*CycleTime+PauseTime)))
  {
    if (Count2 == 0){
//    Serial.println("L2");

    digitalWrite(enablePin,HIGH);
        // Turn channel A1 on
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,LOW);
    digitalWrite(ledPin950, HIGH);
    // Channel A1 ON for LED mux
    digitalWrite(mux1,HIGH); // S0
    digitalWrite(mux2,LOW);  // S1
    digitalWrite(mux3,LOW);  // S2
    digitalWrite(enablePin, LOW);
//    delay(5); 
    }



    // Logic for multiplexer switching the LEDS (850nm)

//    digitalWrite(enablePin_LED, LOW);  

    delay(readSpeed);
    
    add_to_list(valList850, time850,1);
    ++Count2;

    // Turn Channel A1 off for LED mux

  }

  //interval between LED switch
  else if ((millis() - Start) >= ((2*CycleTime)+(PauseTime)) && (millis() - Start) < ((2*CycleTime)+(2*PauseTime)))
  {
    // for switches logic
//    digitalWrite(enablePin_LED, HIGH);
    digitalWrite(enablePin, HIGH);
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950, HIGH);    
  }
  
  //For third LED  
  else if ((millis() - Start >= ((2*CycleTime)+(2*PauseTime))) && (millis() - Start < ((3*CycleTime)+(2*PauseTime))))
  {
    if (Count3 == 0)
    {
//    Serial.println("L3");

    digitalWrite(enablePin, HIGH);
    digitalWrite(ledPin670, HIGH);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,LOW);
    // Channel A2 ON for LED mux
    digitalWrite(mux1,LOW);  //S0
    digitalWrite(mux2,HIGH); //S1
    digitalWrite(mux3,LOW);  //S2
    digitalWrite(enablePin, LOW);
//    delay(5);
    }

  /// for switches logic
  // turn channel A2
//    digitalWrite(driverPin, HIGH);
    


    delay(readSpeed);


    add_to_list(valList950, time950,2);
    ++Count3;

    // Turn Channel A2 off for LED mux
//    digitalWrite(enablePin,HIGH);
  }

  //Process collected data, reset all values, reset timer
  else
  {
    // for switches logic
//    digitalWrite(enablePin_LED, HIGH);
    digitalWrite(enablePin, HIGH);
//    digitalWrite(driverPin, LOW);
    digitalWrite(ledPin670, HIGH);
    digitalWrite(ledPin850, HIGH);
    digitalWrite(ledPin950,HIGH);
   
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

//    digitalWrite(ledPin670, HIGH);
//    digitalWrite(ledPin850,HIGH);
//    digitalWrite(ledPin950,HIGH);
//    digitalWrite(enablePin, LOW);
    
    Count = 0;
    Count2 = 0;
    Count3 = 0;
    Start = millis();

  //  digitalWrite(enablePin,HIGH);
    }
    
}
