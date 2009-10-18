#include  "config.h"
#include <adc.h>


unsigned short invert_adc(unsigned short adc_duty);


#define ADC_PWMPIN	    P2_5
#define ADC_SIGNAL  	P2_6


 
void adc_init(){
   ADC_PWMPIN = 0;
   ADC_SIGNAL = 1;
}

unsigned short xadc_duty=0;
unsigned short xadc_free=ADC_CYCLE;

/*
 * loop : 15 seems better, 20 reliable 
 */
unsigned short adc(unsigned short adc_duty,unsigned short loop)
{
   unsigned short adcloop;
  
   ADC_PWMPIN = 0;
   ADC_SIGNAL = 1;

   //mdelay(1000); //uncharge

   // free+6us     
   for(; adc_duty<= (ADC_CYCLE); adc_duty+=ADC_STEP){

	  xadc_free = ADC_CYCLE-adc_duty;

	 	 
	  adcloop =loop;																					     
      while(adcloop){ 
      	          
		 ADC_PWMPIN = 1;   
		 udelay(adc_duty);
	
		 ADC_PWMPIN = 0;

	  	 udelay(xadc_free);
	  
		 adcloop--;
	  }

	  if(0 == ADC_SIGNAL) {
	      // printhex(adc_duty);
		   return adc_duty;
	  }	

   }
   

   return adc_duty;//adc_duty;
      
}


