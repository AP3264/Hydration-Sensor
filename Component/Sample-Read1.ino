unsigned long Start;
unsigned short Count;
int valCycle[];

void setup( void )
{
  Serial.begin( 9600 );
  Start = millis();
}

void loop( void )
{
  if ( millis() - Start < 100 )
  {
    analogRead( A0 );
    ++Count;
  }
  else
  {
    Serial.println( Count );
    Count = 0;
    Start = millis();
  }
}
