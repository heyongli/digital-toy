#include "config.h"
#include <adc.h>
#include <adc0832.h>

#include <charger.h>

#include <1602.h>


extern float  refV ; 


i_charger charger = INIT_CHARGER;


void io_init()
{
   lcd_init();
   mdelay(100);
   lcd_logo();
}
void main()
{
   unsigned short n,x;
   unsigned short onduty=0;


   pwm_init(); //pull down pwm
   /*power on test*/
   io_init();
	   
   timer0_init();
   timer1_init();

   //adc_init(); 
   irqon();   //enable global interrupt		
   sleep(0); // just refrence 

   while(1){ 
        n = keyscan();
 		if(n == 15){	// key F
			 pwm_setduty(onduty);
			 onduty = 0;
		}
 		if(n!=-1)
		{
           if(n<10)
		      if( 9==n)
			     x=0;
			  else
		         x=(n+1);	
		    onduty = 10*onduty+x;   
		}

	
        charging(&charger);  //charger auto control

     
  }
}
