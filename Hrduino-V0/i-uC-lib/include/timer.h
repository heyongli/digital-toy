#ifndef _uC_TIMER_
#define _uC_TIMER_


#include <util/delay.h> //AVR gcc delay libs

/*  
 * tick/jiffers system 
 */
#define HZ  1000  
extern volatile unsigned  long jiffers;


#define timeafter(a,b)         \
         (((long)(b) - (long)(a) < 0))


void _delay_s(unsigned char s);
void delay(unsigned long ticks);




#endif
