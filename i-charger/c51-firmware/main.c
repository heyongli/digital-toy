#include "config.h"
#include <adc.h>
#include <adc0832.h>

#include <math.h>
#include <charger.h>


 

static unsigned short fx,n,x,adc_i=0,adc_v=0;
static unsigned short onduty=0;

float charging_current=0.0, battery_voltage=0.0;
float  refV=4.04;

void batteryAV(){

      static unsigned long lasttimeA=0,lasttimeV=0;
 	  float tmp=0;
	 
      if(timeafter(jiffers,lasttimeA+HZ/4) ){ 
      
         /*sample the charger current*/
	     adc_i = adc0832(0);
  	  	tmp= ((float)adc_i/255)*refV;
	  	tmp = tmp/18;
	  	tmp = tmp/0.10;
	  	tmp*=1000; //mA
	  	 charging_current=(short)tmp;
		 lasttimeA = jiffers;
	  }
	  /*sample ther battery voltage*/
	  if(timeafter(jiffers,lasttimeV+2*HZ) ){ 
		  pwm_safeoff();  //disalbe charge current
		  mdelay(255);
		  mdelay(255);
		    
      	  adc_v = adc0832(1);  
      	  pwm_safeon();
	  	  tmp=(((float)adc_v)/255)*refV*1.9*100; //10mV
	      battery_voltage =(short)tmp;
		  lasttimeV = jiffers;

      }
	  

}



void icharger_testcontrolkey()
{
  
  	if(10==fx){ // key a
	      vledmod(VLED_A); 
  	      print10((short)charging_current);
  		  setdot(0);
 	}
	
	if(11==fx){ // key b 
 	     vledmod(VLED_V); 
	     print10(battery_voltage);
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

	if(14==n){ // key e
			  refV=5.03;
	}

	if(15==fx){ // key f
	    print10(onduty);
		vledmod(VLED_HZ); 
	}

}
void main()
{
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

	    batteryAV(); //will stop charging for 0.3S
        charging();   //charger auto control
		   
        n = keyscan();
		if(n<=15 && n>=10) /*A .. F*/
		   fx = n;
        
		switch(fx);
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

		icharger_testcontrolkey();
  	    update_vled(); //vled mode 
     
  }
}
