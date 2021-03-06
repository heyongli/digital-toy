#include "include/avrio.h"
#include <avr/interrupt.h>

/* timer0,  fcpu=10Mhz 
 * prescaer:clkio/64, 10M/64= 156250HZ, 
 * 1 ticks= 1000/156250=0.0064ms, 
 * 1ms ticks:1/0.0064=156.25
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
      //_toggle_bits(LED_PORT,1); //hart beats
	  mark = jiffers;
   }

   TCNT0  = TIMER0_TCN;
   return;
} 



/*
from mega88:
----mode CTC---
CTC timer  TOP:OCR0A  update OCRx at: Immediate  TOV flag set on:MAX
CTC opration: TCNT0 count up to OCR0A then been cleared
   
*/
void timer0_init()
{

#if defined (__AVR_ATmega8__)
  //mega8 timer0 only used for count: always up to MAX (0xFF) and start again from 0
  TCCR0  =   (_bits8(0b11,CS00,CS02)) ; //clk(io)/64
  TCNT0  = TIMER0_TCN; //1ms per interrupt
 
  TIMSK |= _bits8(1,TOIE0,TOIE0); //enable overflow interrupt;
  
  //TIFR TOV0, clear by interrupt
   SFIOR |=  PSR10;
#elif  defined (__AVR_ATmega88__)
   //mega88 timer0  normal mode:  always up to MAX (0xFF) and start again from 0
   TCCR0A  =   0;  
   TCCR0B  =  (_bits8(0b11,CS00,CS02)) ;//clk(io)/64
   TCNT0  = TIMER0_TCN; //1ms per interrupt
 
   TIMSK0 |= _bits8(1,TOIE0,TOIE0); //enable over flow interrupt;
  
  //prescaler reset
   GTCCR  |=  PSRSYNC;
#else
   #error "cpu not support !"
#endif

}


void enable_timer0()
{
#if defined (__AVR_ATmega88__)
   TIMSK0 |= _bits8(1,TOIE0,TOIE0);;// enable interrupt;
#elif  defined (__AVR_ATmega8__)
   TIMSK |= _bits8(1,TOIE0,TOIE0);;// enable interrupt;
#else
   #error "cpu not support !"
#endif 

}
void disable_timer0()
{
#if defined (__AVR_ATmega88__)
    TIMSK0  &= ~_bits8(1,TOIE0,TOIE0);// enable interrupt;
#elif defined (__AVR_ATmega8__)
    TIMSK  &= ~_bits8(1,TOIE0,TOIE0);// enable interrupt;
#else
   #error "cpu not support !"
#endif
}
