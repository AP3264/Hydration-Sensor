#include <LinkedList.h>

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

void setup( void )
{
  Serial.begin( 38400 );
  while(!Serial){
    }
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
  if ( millis() - Start < 100 )
  {
    //Serial.read();
    Serial.println("123");
    digitalWrite(ledPin670,LOW);
    analogRead( A0 );
    valList670.add(analogRead( A0 ));
    time670.add(micros() - timer);
    ++Count;
  }
  else if ((millis() - Start >= 100) && (millis() - Start < 110)){
    digitalWrite(ledPin670,HIGH);
    }
  else if ((millis() - Start >= 110) && (millis() - Start < 220))
  {
//    Serial.read();
    Serial.println("12124");
    digitalWrite(ledPin670,HIGH);
    digitalWrite(ledPin850,LOW);
    int x = analogRead( A0 );
    valList850.add(x);
    time850.add(micros() - timer);
    ++Count2;
  }
  else if ((millis() - Start >= 220) && (millis() - Start < 230)){
    digitalWrite(ledPin850,HIGH);
    }
  else if ((millis() - Start >= 230) && (millis() - Start < 330))
  {
    //Serial.read();
    Serial.println("12413");
    digitalWrite(ledPin850,HIGH);
    digitalWrite(ledPin950,LOW);
    analogRead( A0 );
    valList950.add(analogRead( A0 ));
    time950.add(micros() - timer);
    ++Count3;
  }
  else
  {
    digitalWrite(ledPin950,HIGH);
    //Serial.println(millis() - Start);
    Serial.println("count:");
    Serial.println( Count );
    Serial.println( Count2 );
    Serial.println( Count3 );
    arrayLoop(valList670, time670,10);
    arrayLoop(valList850, time850,10);
    arrayLoop(valList950, time950,10);

   int listSize = valList670.size();
   int listSize1 = valList850.size();
   int listSize2= valList950.size();
   //Serial.println(time670.size());
    
//    for (int h = 0; h < listSize; h+=2) {
//      if (h <= listSize){
//        int val = valList670.get(h);
//        unsigned long tim = time670.get(h);
//        Serial.print(tim);
//        Serial.print("\t\t");
//        Serial.println(val);
//      }
//    }
//      
//    for (int h = 0; h < listSize1; h+=2) {
//      if (h <= listSize1){
//        int val = valList850.get(h);
//        unsigned long tim = time850.get(h);
//        Serial.print(tim);
//        Serial.print("\t\t");
//        Serial.println(val);
//      }
//    }
//    for (int h = 0; h < listSize2; h+=2) {
//      if (h <= listSize2){
//        int val = valList950.get(h);
//        unsigned long tim = time950.get(h);
//        Serial.print(tim);
//        Serial.print("\t\t");
//        Serial.println(val);
//      }
//    }
      
    valList670.clear();
    valList850.clear();
    valList950.clear();
    time850.clear();
    time670.clear();
    time950.clear();
    timer = micros();
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
        Serial.println(val);
      }
  }
}
