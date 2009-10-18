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
       static unsigned short n,x=0,x1=0;
 	   static unsigned short onduty=0;
   	   static unsigned long lasttime=0;

	   	   
	   	
	   if(timeafter(jiffers,lasttime+1000) ){			    
		
				pwm_safeoff();
				 //fast search
			   	if(onduty>10)
			    	onduty = adc(onduty-5,15);
				else
				    onduty = adc(0, 2);
			    pwm_safeon();

				printhex(onduty);
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
			    pwm_safeoff();
				if( ( onduty< ADC_CYCLE)&&(onduty>8)){
				   x=adc(onduty-8,20);
  				   x1=adc(onduty-8,20);
				}else
				   adc(0,3);
			    pwm_safeon();
				onduty=(x+x1)>>1;
				printhex(onduty);
				
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
