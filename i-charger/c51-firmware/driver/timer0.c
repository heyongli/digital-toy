
#include  "config.h"
#include  "timer.h"

/*
 * timer0, 
 *   1) 1ms ticks
 *	 2) tickless driver 
 */

volatile unsigned  long jiffers=0;  //250us per jiffers


/* 12Mhz  1ms = 1000us
 *    1ms jiffers 
 *    10kHz PWM (100us PWM cycle,if<50us onduty use busy wait)
 *    0-125us on-duty      
 */
/*下一个interval是上一个的 1/n */
#define MS_CYCLE  1000
#define PWM_CYCLE 100		  /*100+[~140] cycles*/ //give up 1000+ left_cpu_of_timer



#define settimer0(us) TH0 = (65536 - (us))>>8 ; /* (655535-us)/256 超慢!!!!*/  \
                      TL0 = (65536 - (us))&0xFF ; 

#define settimer1(us) TH1 = (65536 - (us))>>8 ; /* (655535-us)/256 超慢!!!!*/  \
                      TL1 = (65536 - (us))&0xFF ; 
 


void timer0_init()
{

   /*  timer 0 in mode 1   */
   TMOD |= 0x2;   
      
   //settimer0(PWM_CYCLE);
   TH1 = TH0 = 200; //100us  15 step , 600Hz
   // enable timer 0 interrupt   
   ET0 = 1;   
   TR0 = 1; //enable timer

}

				 
/*
 * Timer0: 256us, display driven ticks
 */
/*中断1， 寄存器组1 (普通函数默认用寄存器组0)*/



void timer0(void) interrupt 1 using 2   
{   

	pwm_1kHz();

#if 0
    if(PWM_PIN) { //need close PWM
   
   	   PWM_PIN = 0;
	   settimer0(PWM_CYCLE-on_duty);
     //  vledx0();

	 
     }else { //need open PWM
  
       PWM_PIN = 1;
	   settimer0(on_duty);//on_duty);
//	   vledx1();

     }  
#endif

}  


void timer1_init()
{
  /* timer0 init */
   /*  timer 0 in mode 2   */
   TMOD |= 0x10;   
   // set timer speed   
   settimer1(MS_CYCLE);   	//per 250us

  
   // enable timer 1 interrupt   
   ET1 = 1;   
   
   TR1 = 1; //enable timer

}

void timer1(void) interrupt 3 using 1   
{   
 	 ms_scan_segvled(); //扫描显示

	 
	 jiffers++;
	 settimer1(1000);
}




