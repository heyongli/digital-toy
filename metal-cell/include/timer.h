#ifndef _uC_TIMER_
#define _uC_TIMER_

/*
 *   timer.h
 *	 provide : _delay_ms, _delay_us,_delay_s,jiffers, HZ, nop
 *
 */
#include <config.h> //system resource allocation and define

#ifdef ARCH_AVR
#include <util/delay.h> //AVR gcc delay libs
#endif 

#ifdef ARCH_C51
#include <c51/delay.h> //AVR gcc delay libs
#endif 


/*  
 * tick/jiffers system 
 */
#define HZ  1000  
extern volatile unsigned  long jiffers;


#define timeafter(a,b)         \
         (((long)(b) - (long)(a) < 0))


void _delay_s(unsigned char s);


#endif
