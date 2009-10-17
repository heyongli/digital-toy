#include "config.h"
#include <adc.h>

#include <intrins.h>
 


void main()
{
   pwm_init(); //pull down pwm
   /*power on test*/
 
   segvled_init();

   timer0_init();
   timer1_init();

   adc_init(); 
   vledmod('A');

   irqon();   //enable global interrupt		
   
   while(1){ 
       unsigned char n,x;
 	   static unsigned short onduty=0;
   	         static unsigned long lasttime=0;

		
		
	   if(timeafter(jiffers,lasttime+800) ){			    
		
				pwm_safeoff();
				x=adc(0);
			    pwm_safeon();
				//printhex(x);
				vledmod('H'); 
				 lasttime = jiffers;
  	    }

        n = keyscan();

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
			  printhex(onduty);
		   }

           if(10==n) // key A 
 		      printhex(pwm_getduty());
  
		    if(11==n) // key B 
 		       vledmod('A'); 
	   
	   		if(12==n){ // key C  
			  //  unsigned short x;
			    pwm_safeoff();
				if(onduty< ADC_CYCLE)
				   adc(onduty);
				else
				   adc(0);
			    pwm_safeon();
				//printhex(x);
				vledmod('A'); 
	              
			}
			if(13==n){ // key d 
 		        vledmod('V'); 
			    pwm_safeoff();
			  //	invert_adc();
			    pwm_safeon();
				//printhex(x);
			
	            
	   		}
			if(15==n){ // key d 
 		       
	             
	   		}		
	   
		}
		   
		
	    //segvled_demo();
  	    update_vled(); //vled mode 
     
  }
}
