#ifndef _TIMER_H_
#define _TIMER_H_

/* 20Mhz  1ms = 1000us
 *    5ms jiffers 
 *    10kHz PWM (100us per pusle,25step, 40us mini-cycle,PWMCYCLE=400us/0.6 )
 *    0-125us on-duty      
 */
#define MS_CYCLE  8330 /*5ms*/
#define PWM_CYCLE 60   /*must >50, < 50 will consume all cpu*/	  /*100+[~140] cycles*/ //give up 1000+ left_cpu_of_timer
#define PWM_STEP  20  /*one cell get 10 mA at duty<4*/

#define HZ  (((unsigned long)1000*1000)/(unsigned long)MS_CYCLE) /*jiffers per MS*/
             //(1000*1000)/MS_CYCLE, 不行... 数据类型可能是char...


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

#define cli()    EA = 0;   //disable global interrupt	   
#define sti()    EA = 1;   //enable global interrupt	   

     
#endif