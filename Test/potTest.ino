#include <SPI.h>
byte address = 0x11;
int CS= 4;
int i=0;
int pin = 3;

void setup()
{
 pinMode (CS, OUTPUT);
 pinMode (pin, OUTPUT);
 digitalWrite(pin, HIGH);
 Serial.begin( 9600 );
 SPI.begin();
 // adjust high and low resistance of potentiometer
 // adjust Highest Resistance .
  digitalPotWrite(0x00);
  delay(1000);

     // adjust  wiper in the  Mid point  .
  digitalPotWrite(0x80);
  delay(1000);

  // adjust Lowest Resistance .
  digitalPotWrite(0xFF);
  delay(1000);

  
}

void loop()
{
   for (i = 255; i >= 0; i -= 10)
   {
     digitalWrite(pin, LOW);
     delay(1000);
     digitalPotWrite(i);
     digitalWrite(pin, HIGH);
     delay(3000);
     Serial.println(i);
     delay(10);
     
   }
   delay(100);
   for (i = 0; i <= 255; i += 10)
   {
     digitalWrite(pin, LOW);
     delay(1000);
     digitalPotWrite(i);
     digitalWrite(pin, HIGH);
     delay(3000);
     Serial.println(i);
     delay(10);
     
   }
//     digitalWrite(pin, LOW);
//     delay(1000);
//     digitalPotWrite(100);
//     digitalWrite(pin, HIGH);
//     delay(3000);
//     Serial.println(i);



//digitalPotWrite(255);
//delay(5000);
//digitalWrite(pin, LOW);
//delay(100);
//digitalWrite(pin, HIGH);
//delay(100);
//digitalPotWrite(100);
//delay(5000);
//digitalWrite(pin, LOW);
//delay(100);
//digitalWrite(pin, HIGH);
//delay(100);
//digitalPotWrite(40);
//delay(5000);
}

int digitalPotWrite(int value)
{
 digitalWrite(CS, LOW);
 SPI.transfer(address);
 SPI.transfer(value);
 digitalWrite(CS, HIGH);
}
