#include <LinkedList.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 adc;

LinkedList<int> valList670;

LinkedList<unsigned long> time670;

unsigned long Start;
unsigned short Count;

unsigned long timer;

#define ledPin670 13

#define readSpeed 5
#define CycleTime 100
#define PauseTime 10

int adc0;

void setup( void )
{
  Serial.begin( 38400 );
  while(!Serial){}
  Wire.begin();

  adc.begin();
  
  pinMode (ledPin670, OUTPUT);

  digitalWrite(ledPin670,HIGH);
  
  Start = millis();
  timer = micros();
}

void set_pot_val(int byteVal)
{
    Wire.write(byte(0x00));
    Wire.write(byte(byteVal));
    Wire.endTransmission();
}

void add_to_list(LinkedList<int> &pinNum,LinkedList<unsigned long> &timerList)
{
    pinNum.add(adc.readADC_SingleEnded(0));
    timerList.add(micros() - timer);
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

    if (Count == 0)
    {
      Serial.println(" ");
    }
    delay(readSpeed);
    digitalWrite(ledPin670,LOW);
    adc0 = adc.readADC_SingleEnded(0);
    add_to_list(valList670, time670);
    ++Count;
  }
  else
  {
    digitalWrite(ledPin670,HIGH);

    //Print all values to serial port
    print_to_ser(valList670, time670,1);

    //Reset all list to empty
    valList670.clear();
    time670.clear();

    //Reset Counters to 0 and restart timer
    Count = 0;
    Start = millis();

    //Ensure all LEDs off
    digitalWrite(ledPin670,HIGH);
    
  }
}
