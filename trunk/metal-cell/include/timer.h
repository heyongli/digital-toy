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
extern volatile unsigned  long jiffers;

#ifdef ARCH_AVR
#define HZ  1000  
#endif

#ifdef ARCH_C51
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
#endif



#define timeafter(a,b)         \
         (((long)(b) - (long)(a) < 0))


void _delay_s(unsigned char s);


#endif
