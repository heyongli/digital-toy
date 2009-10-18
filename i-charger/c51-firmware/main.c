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
       static unsigned short n,x,adc=0,adc_v=0;
 	   static unsigned short onduty=0;
   	   static unsigned long lasttime=0;

	   	   
	   	
	   if(timeafter(jiffers,lasttime+1000) ){			    
		
				pwm_safeoff();
				if(0==adc) {
				   adc = pwm_fadc();
			    }
				 //fast search
			   	if(adc>8){
				     x = pwm_adc(adc-5);
					 x+= pwm_adc(adc-5);
					 x+= pwm_adc(adc-5);
					 x+= pwm_adc(adc-5);
					 adc=x>>2;

				    //V= 600+((adc-24)/8)*50
					{
					  float v=0;
					  v= 650+ 6.25*((float)adc-24);
					  v=(v/(24.917) )*10+45; //get xxx mA ,45 is delta of system
					  adc_v=(unsigned short)v;
					}
				}else
                     adc = pwm_adc(0);
			    pwm_safeon();

				printhex(adc_v);
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
		        adc = pwm_adc(pwm_fadc());
				pwm_safeon();
				printhex(adc);
				vledmod('A'); 
	              
			}
			if(13==n){ // key d 
 		        vledmod('V'); 
	            
	   		}
			if(15==n){ // key d 
 		       
	             
	   		}		
	   
		}
		   
		
	    //segvled_demo();
  	    update_vled(); //vled mode 
     
  }
}
