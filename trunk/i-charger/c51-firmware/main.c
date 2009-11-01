#include "config.h"
#include <adc.h>
#include <adc0832.h>

#include <math.h>
#include <charger.h>




extern float  refV ; 


i_charger charger = INIT_CHARGER;


char action= 0;

void  update_display()
{
  
    static unsigned long start=0;
  	if(10==action){ // key a
	    //vledmod(VLED_A); 
  	    // print10((short)charging_current);
        start = jiffers;
		action = 12	;
 	}
	
	if(11==action){ // key b 
 	     //vledmod(VLED_V); 
	    // print10((short)(battery_voltage*100));
		unsigned long total = (((long)(jiffers) - (long)(start) ));

		float x = ((float)total/(HZ*60.0));

		print10(10*x); //minute
        setdot(2);

	} 
		
	if(12==action){ //key c
	  //count second
	   setdot(2);
	   print10( (jiffers-start)/(HZ/10) );  

	}


	if(14==action){ // key E
	   refV=5.03;
	}
    if(15==action){ //key f
  	    ic_show(&charger);
    }

}
void main()
{
   unsigned short n,x;
   unsigned short onduty=0;
   static unsigned long lasttime= 0;

   pwm_init(); //pull down pwm
   /*power on test*/
   segvled_init();

   timer0_init();
   timer1_init();

   //adc_init(); 
   irqon();   //enable global interrupt		
   sleep(0); // just refrence 

   while(1){ 
        n = keyscan();

		if(n<=15 && n>=10) /*A .. F*/
		   action = n;
        
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

		if(13==action){ //key d
           charger.dump++;
		   if(charger.dump>=ICS_END)
		      charger.dump=ICS_DEFAULT;
		   action = 15;
	    }   
        charging(&charger);  //charger auto control

		if(timeafter(jiffers, lasttime+HZ/10)){
			 update_display();
			 update_vled(); //vled mode 
			 lasttime = jiffers;
		}
     
  }
}
