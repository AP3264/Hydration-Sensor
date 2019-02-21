#include <LinkedList.h>
#include <Wire.h>

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

void setup( void )
{
  Serial.begin( 38400 );
  while(!Serial){
    }
  Wire.begin();
  pinMode (ledPin670, OUTPUT);
  pinMode (ledPin850, OUTPUT);
  pinMode (ledPin950, OUTPUT);
  digitalWrite(ledPin670,HIGH);
  digitalWrite(ledPin850,HIGH);
  digitalWrite(ledPin950,HIGH);
  Start = millis();
  timer = micros();
}

void loop( void )
{
  Wire.beginTransmission(44);
  
  if ( millis() - Start < 100 )
  {
    Wire.write(byte(0x00));
    Wire.write(byte(21));
    Wire.endTransmission();
    //Serial.read();
    if (Count == 0){
    Serial.println(" ");
    }
    delay(5);
    digitalWrite(ledPin670,LOW);
    analogRead( analogIn );
    valList670.add(analogRead( analogIn ));
    time670.add(micros() - timer);
    ++Count;
  }
  else if ((millis() - Start >= 100) && (millis() - Start < 110)){
    digitalWrite(ledPin670,HIGH);
    }
  else if ((millis() - Start >= 110) && (millis() - Start < 210))
  {
    Wire.write(byte(0x00));
    Wire.write(byte(27));
    Wire.endTransmission();
//    Serial.read();
    if (Count2 == 0){
    Serial.println(" ");
    }
    //Serial.println(" ");
    delay(5);
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,LOW);
    analogRead( analogIn );
    valList850.add(analogRead(analogIn));
    time850.add(micros() - timer);
    ++Count2;
  }
  else if ((millis() - Start >= 210) && (millis() - Start < 220)){
    digitalWrite(ledPin850,HIGH);
    }
  else if ((millis() - Start >= 220) && (millis() - Start < 320))
  {
    Wire.write(byte(0x00));
    Wire.write(byte(29));
    Wire.endTransmission();
    //Serial.read();
    if (Count3 == 0){
    Serial.println(" ");
    }
    delay(5);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,LOW);
    analogRead( analogIn );
    valList950.add(analogRead( analogIn ));
    time950.add(micros() - timer);
    ++Count3;
  }
  else
  {
    digitalWrite(ledPin950,HIGH);
    //Serial.println(millis() - Start);
//    Serial.println("count:");
//    Serial.println( Count );
//    Serial.println( Count2 );
//    Serial.println( Count3 );
    arrayLoop(valList670, time670,1);
    arrayLoop(valList850, time850,1);
    arrayLoop(valList950, time950,1);

   int listSize = valList670.size();
   int listSize1 = valList850.size();
   int listSize2= valList950.size();
   //Serial.println(time670.size());
      
    valList670.clear();
    valList850.clear();
    valList950.clear();
    time850.clear();
    time670.clear();
    time950.clear();
    //timer = micros();
    Count = 0;
    Count2 = 0;
    Count3 = 0;
    Start = millis();
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,HIGH);
    
  }
}

void arrayLoop(LinkedList<int> &pinNum,LinkedList<unsigned long> &timer, int valDiff){
//  Serial.println(pinNum);
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
