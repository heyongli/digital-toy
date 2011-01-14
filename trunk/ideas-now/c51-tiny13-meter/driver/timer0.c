
#include  "config.h"
#include  "timer.h"

/*
 * timer0, 
 *   1) 1ms ticks
 *	 2) tickless driver 
 */

volatile unsigned  long jiffers=0;  //250us per jiffers





#define settimer0(us) TH0 = (65536 - (us))>>8 ; /* (655535-us)/256 ³¬Âý!!!!*/  \
                      TL0 = (65536 - (us))&0xFF ; 

#define settimer1(us) TH1 = (65535 - (us))>>8 ; /* (655535-us)/256 ³¬Âý!!!!*/  \
                      TL1 = (65535 - (us))&0xFF ; 
 


void timer0_init()
{
  /*mode1: 16bit, need reload*/
   /*mode2: 8bit, auto reload*/
   /*  timer 0 in mode 1   */
   
   TMOD |= 0x2;   
      
   //settimer0(PWM_CYCLE); //mode1
   TH1 = TH0 = (256-PWM_CYCLE); //100us  15 step , 600Hz
   // enable timer 0 interrupt   
   ET0 = 1;   
   TR0 = 1; //enable timer

}

				 
/*
 * Timer0: 256us, display driven ticks
 */
/*ÖÐ¶Ï1£¬ ¼Ä´æÆ÷×é1 (ÆÕÍ¨º¯ÊýÄ¬ÈÏÓÃ¼Ä´æÆ÷×é0)*/

extern unsigned char mode;
extern unsigned char on_duty ;
void timer0(void) interrupt 1 using 2   
{   

     


}  


void timer1_init()
{
  /* timer0 init */
   /*  timer 0 in mode 1   */
   TMOD |= 0x10;   
   // set timer speed   
   settimer1(MS_CYCLE);   	//per 250us

  
   // enable timer 1 interrupt   
   ET1 = 1;   
   
   TR1 = 1; //enable timer

}

void timer1(void) interrupt 3 using 1   
{   
 	 ms_scan_segvled(); //É¨ÃèÏÔÊ¾, use LCD disable this
		 
	 jiffers++;
	 settimer1(MS_CYCLE);
}




