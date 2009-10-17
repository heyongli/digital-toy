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

unsigned short adc(unsigned short adc_duty)
{

 
   ADC_PWMPIN = 0;
   ADC_SIGNAL = 1;

   //mdelay(200); //uncharge

     
   for(/*(adc_lastduty)*/; adc_duty<= (ADC_CYCLE); adc_duty+=ADC_STEP){

	  
	  unsigned short loop=50; //200 PWM cycle,0.5s
      while(loop){
                  
		 ADC_PWMPIN = 1;   

		 if(adc_duty>3)
		    udelay(adc_duty);

		 ADC_PWMPIN = 0;

	  	 udelay(ADC_CYCLE-adc_duty);
 
	  
		 loop--;
	  }

	  if(0 == ADC_SIGNAL) {
	       //unsigned short invert=0;
	       //invert=invert_adc(adc_duty+10);
		   //if(invert != 0xFFFF)
		     //adc_duty = (invert+adc_duty)>>1;
	       printhex(adc_duty);
		   return adc_duty;
	  }	
   
   }
   

   //(5x1000 *adc_duty )/128
   //return (5100*adc_duty)>>7;
   printhex(1234);
   return 0;//adc_duty;
      
}

unsigned short invert_adc(unsigned short adc_duty)
{

   ADC_PWMPIN = 1;
   ADC_SIGNAL = 1;

   for(; adc_duty>=0; adc_duty--){
      unsigned short loop=100; //200 PWM cycle,0.5s

      while(loop--){
                  
		 ADC_PWMPIN = 1;
		 if(adc_duty>3)
		    udelay(adc_duty);
		 udelay(adc_duty);
		 ADC_PWMPIN = 0;
	
		 udelay(ADC_CYCLE-adc_duty);
	  }
	  
      if(1 == ADC_SIGNAL) {
  	  	  // printhex(adc_duty);
		   return adc_duty;
      }
 
   
   }
   

   //(5x1000 *adc_duty )/128
   //return (5100*adc_duty)>>7;
   printhex(1234);
   return 0xFFFF;//adc_duty;

}

