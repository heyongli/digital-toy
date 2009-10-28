#include "config.h"
#include <adc.h>
#include <adc0832.h>

#include <math.h>
#include <charger.h>


 

static unsigned short n,x,adc_i=0,adc_v=0;
static unsigned short onduty=0;

extern float charging_current, battery_voltage;
extern float  refV ; 




void icharger_testcontrolkey(char fx)
{
  
  	if(10==fx){ // key a
	      //vledmod(VLED_A); 
  	     // print10((short)charging_current);
  		  setdot(0);
 	}
	
	if(11==fx){ // key b 
 	     //vledmod(VLED_V); 
	    // print10((short)(battery_voltage*100));
	     setdot(1);	

	} 
		
	if(12==fx){ //key c
	     print10(adc_i);
	     setdot(-1);
	}
	if(13==fx){ //key d
			setdot(-1);
			print10(adc_v);
	}   

	if(14==fx){ // key e
	   refV=5.03;
	}

	if(15==fx){ // key f
	    print10(onduty);
		//vledmod(VLED_HZ); 
	}

}
void main()
{
   char fx= 0;
   pwm_init(); //pull down pwm
   /*power on test*/
 
   segvled_init();

   timer0_init();
   timer1_init();

   //adc_init(); 
   fx = 11;
   irqon();   //enable global interrupt		
   sleep(0); // just refrence 
   while(1){ 

	    charging();   //charger auto control
		   
        n = keyscan();
		if(n<=15 && n>=10) /*A .. F*/
		   fx = n;
        
		
 		if(n!=-1)
		{
  		   if(n == 15){	// key F
			 pwm_setduty(onduty);
			 onduty = 0;
           }else if(n<10) {	
		      if( 9==n)
			     x=0;
			  else
		         x=(n+1);	
		   	  onduty = 10*onduty+x;   
			  print10(onduty);
		   }
      
		}

		//icharger_testcontrolkey(fx);
  	    update_vled(); //vled mode 
     
  }
}
