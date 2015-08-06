#include <TimerOne.h>

const byte IRMask = B00000001;

const int HeaderHigh = 2000;
const int HeaderLow = 2000;
const int Pulse = 300;
const int ZeroLow = 280;
const int OneLow = 660;
const int HalfCarrier = 14;
const unsigned long CommandLength = 120000;

unsigned long elapsed = 0;

void switchOn();
void switchOff();

void switchCarrier();

void loopDelayMicroseconds(unsigned long);

boolean on = false;

unsigned long msg = 0b00111100001111111100100000110100;

void setup()
{
  DDRB |= B00000001;
  Timer1.initialize(HalfCarrier);
  Timer1.attachInterrupt(switchCarrier);
}

void loop()
{
  unsigned long msgTemp = msg;
  
  unsigned long ti = micros();
  
  on = true;
  loopDelayMicroseconds(HeaderHigh);
  on = false;
  loopDelayMicroseconds(HeaderLow);
  on = true;
  loopDelayMicroseconds(Pulse);
  
  for(unsigned int i = 0; i < 32; ++i)
  {
    on = false;
    if((msgTemp & 0x80000000ULL) == 0x80000000ULL)
    {
      loopDelayMicroseconds(OneLow);
    }
    else
    {
      loopDelayMicroseconds(ZeroLow);
    }
    
    on = true;
    loopDelayMicroseconds(Pulse);
    
    on = false;
    
    msgTemp <<= 1;
  }
  
  loopDelayMicroseconds(CommandLength - (micros() - ti));
}

void switchCarrier()
{
  static boolean last = false;
  last = !last;
  if(on)
  {
    if(!last)
    {
      switchOff();
    }
    else
    {
      switchOn();
    }
  }
  else
  {
    switchOff();
  }
}

void switchOn()
{
  PORTB |= IRMask;
}

void switchOff()
{
  PORTB &= ~IRMask;
}

void loopDelayMicroseconds(unsigned long us)
{
  unsigned long ti = micros();
  while(micros() - ti < us);
}
