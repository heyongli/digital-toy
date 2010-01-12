#include "include/avrio.h"
#include <avr/interrupt.h>

/* timer0,  fcpu=4Mhz, 
 * prescaer:clkio/64, 4M/64= 62500HZ, 
 * 1 ticks= 1000/62500=0.016ms, 
 * 1ms ticks:1/0.016=62.5
 */
#define TIMER0_HZ  ((F_CPU)/(64UL))
#define TIMER0_TCN (255UL- ((TIMER0_HZ)/(HZ)) )  /*62.5 times OVERFLOW, 0.5 for ISR overhead...*/


/*
 * timer0,1ms ticks
 *
 */
volatile unsigned  long jiffers=0;  //250us per jiffers

#define LED_PORT PORTD


ISR(TIMER0_OVF_vect)
{


   jiffers++;
   static unsigned long mark= 0;
   if(timeafter(jiffers,mark+ HZ/8 )){ //4times per second
      _toggle_bits(LED_PORT,0b11); //hart beats
	  mark = jiffers;
   }

   TCNT0  = TIMER0_TCN;
   return;
} 



void timer0_init()
{

  TCCR0  =   (_bits8(0b11,CS00,CS02)) ;
  TCNT0  = TIMER0_TCN; //1ms per interrupt
 
  //TIMSK |= _bits8(1,TOIE0,TOIE0); //enable interrupt;
  
  //TIFR TOV0, clear by interrupt
 // SFIOR |=  PSR10;

}


void enable_timer0()
{
   TIMSK |= _bits8(1,TOIE0,TOIE0);;// enable interrupt;

}
void disable_timer0()
{
   TIMSK  &= ~_bits8(1,TOIE0,TOIE0);// enable interrupt;
}
