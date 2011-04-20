
#ifndef _METAL_C51_DELAY_H_
#define _METAL_C51_DELAY_H_

#include <config.h>  //F_CPU


// _2cycle(1)-> move r7, 1, lcall _2cycle, total 3cycle
//so _2cycle(n)=> 2n+5 cycles, n from 1...255;
#pragma REGPARMS
void _2cycle(unsigned char cyc);


/*12Mhz : at least 7us (7-5=2 2/2=1)   + - 1us if above 8us*/
#define _delay_us(t)  _2cycle((t-5)>>1) //_2cycle can't accept 0


/*
 * always use the constant of __us
 * 1-255 cycles, 2-510us at 12Mhz
 */
#define _xdelay_us(__us)  /* usage: _delay_us(8), wrong: i=0; _delay_us(i);*/ \
do{ \
    unsigned char __ticks; \
	double __tmp = ((F_CPU)/24e6)*__us##UL;  /*24Mhz for 1us ticks*/				\
	if(__tmp<1.0)  																	\
		__ticks=1; 																	\
    else if (__tmp>255)																\
		__ticks = 0;																\
	else 																			\
		__ticks = (unsigned char)__tmp;											    \
    /*12Mhz 2us per ticks*/															 \
    while(--__ticks);	 /*while(--cyc) have bug when cyc==0, but we must use --cyc to genarate one inst*/\
    /* DFFE     DJNZ     R7,delay(C:0014), 2cycle*/														 \
} while(0)


/*
 * mdelay(0) : 15 cycle, 12Mhz 30us
 * 1ms to 256ms
 *  +- 50us
 */
void _delay_ms(unsigned char ms);




/*
 * +- 50ms
 */
void sleep(unsigned char t);

#endif

