#include <LinkedList.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 adc;

int adc0;

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
unsigned long timer;

#define ledPin670 13
#define ledPin850 12
#define ledPin950 11
#define analogIn A2
#define readSpeed 5
#define CycleTime 100
#define PauseTime 10

#define mosfetPin 9

void setup( void )
{
  Serial.begin( 38400 );
  while(!Serial){}
  Wire.begin();
  adc.begin();
  
  pinMode (ledPin670, OUTPUT);
  pinMode (ledPin850, OUTPUT);
  pinMode (ledPin950, OUTPUT);
  pinMode(mosfetPin, OUTPUT);
  
  digitalWrite(ledPin670,HIGH);
  digitalWrite(ledPin850,HIGH);
  digitalWrite(ledPin950,HIGH);
  
  Start = millis();
  timer = micros();
}

void set_pot_val(int byteVal)
{
    Wire.write(byte(0x00));
    Wire.write(byte(byteVal));
    Wire.endTransmission();
}

void add_to_list(LinkedList<int> &pinNum,LinkedList<unsigned long> &timeList)
{
   // pinNum.add(analogRead( analogIn ));
    pinNum.add(adc.readADC_SingleEnded(0));
    timeList.add(micros() - timer);
}

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
        Serial.println((val/1023.0)*5);
      }
  }
  Serial.println(" ");
}

void loop( void )
{
  Wire.beginTransmission(44);
  
  if ( millis() - Start < CycleTime )
  {
    set_pot_val(21);
    digitalWrite(mosfetPin,HIGH);

    if (Count == 0)
    {
      Serial.println(" ");
    }
    delay(readSpeed);
    digitalWrite(ledPin670,LOW);
    adc.readADC_SingleEnded(0);
    add_to_list(valList670, time670);
    ++Count;
    digitalWrite(mosfetPin,LOW);
  }
  
  else if ((millis() - Start >= CycleTime) && (millis() - Start < (CycleTime + PauseTime)))
  {
    digitalWrite(ledPin670,HIGH);
  }

    
  else if ((millis() - Start >= (CycleTime+PauseTime)) && (millis() - Start < (2*CycleTime+PauseTime)))
  {
    set_pot_val(27);

    if (Count2 == 0){
    Serial.println(" ");
    }

    delay(readSpeed);
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,LOW);
    adc.readADC_SingleEnded(0);
    
    add_to_list(valList850, time850);
    
    ++Count2;
  }
  
  else if ((millis() - Start) >= ((2*CycleTime)+(PauseTime)) && (millis() - Start) < ((2*CycleTime)+(2*PauseTime)))
  {
    digitalWrite(ledPin850,HIGH);
  }
    
  else if ((millis() - Start >= ((2*CycleTime)+(2*PauseTime))) && (millis() - Start < ((3*CycleTime)+(2*PauseTime))))
  {
    set_pot_val(29);
    
    if (Count3 == 0)
    {
    Serial.println(" ");
    }
    
    delay(readSpeed);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,LOW);
    adc.readADC_SingleEnded(0);
    
    add_to_list(valList950, time950);
    ++Count3;
  }
  
  else
  {
    digitalWrite(ledPin950,HIGH);

    //Print all values to serial port
    print_to_ser(valList670, time670,1);
    print_to_ser(valList850, time850,1);
    print_to_ser(valList950, time950,1);

    //Reset all list to empty
    valList670.clear();
    valList850.clear();
    valList950.clear();
    time850.clear();
    time670.clear();
    time950.clear();

    //Reset Counters to 0 and restart timer
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
