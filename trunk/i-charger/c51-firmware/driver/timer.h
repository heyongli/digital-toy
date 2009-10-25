#ifndef _TIMER_H_
#define _TIMER_H_

/* 12Mhz  1ms = 1000us
 *    5ms jiffers 
 *    10kHz PWM (100us PWM cycle,if<50us onduty use busy wait)
 *    0-125us on-duty      
 */
#define MS_CYCLE  2000
#define PWM_CYCLE 160		  /*100+[~140] cycles*/ //give up 1000+ left_cpu_of_timer

#define HZ  ((1000*1000/MS_CYCLE)) /*jiffers per MS*/


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