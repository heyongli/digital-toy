#include "include/avrio.h"


/*
 * timer0,1ms ticks
 *
 */
volatile unsigned  long jiffers=0;  //250us per jiffers



/*timer0,  fcpu=4Mhz, prescaer:clki/o/1024=4k, 256us clk, */
void timer0_init()
{

  TCCR0  =   (_bits8(0b101,CS00,CS02) ) ;
  TCNT0  = 4; //1ms per interrupt
 
  TIMSK |= TOIE0; //enable interrupt;
  
  //TIFR TOV0, clear by interrupt

}

#if 0
void enable_timer0()
{
   TIMSK |= TOIE0;// enable interrupt;
}
void disable_timer0()
{
  TIMSK   &= ~TOIE0;// enable interrupt;
}
#endif
