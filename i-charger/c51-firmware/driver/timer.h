#ifndef _TIMER_H_
#define _TIMER_H_

#define timeafter(a,b)         \
         (((long)(b) - (long)(a) < 0))

#define rol8(a,n) \
           ( \
		     (a>>(8-(n))) | \
			 (a<<(n)) \
		   ) 
#define ror8(a,n) \
           ( \
		     (a<<(8-(n))) | \
			 (a>>(n)) \
		   ) 





extern volatile unsigned  long jiffers;

void timer0_init();
void timer1_init();

#define irqoff()   EA = 0;   //disable global interrupt	   
#define irqon()    EA = 1;   //disable global interrupt	   
     
#endif