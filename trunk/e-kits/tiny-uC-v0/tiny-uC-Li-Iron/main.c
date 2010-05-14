#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

#include "tiny-uc.h"

#define uckey_init()  _pin_mode(PORTB,ucKEY,INPUT)

#define _test_key(x,n) 								\
	


char keydown()
{
	if( ! _test_bit(_inb(PORTB),ucKEY)) {			
	    	_delay_ms(7); 							
	    	if(! _test_bit(_inb(PORTB),ucKEY ))		
				return 1;							
        }											
       return 0	;									
}


#ifdef PWM
#define ccstart()  pwm_setduty(duty) //_clear_bit(PORTB,ucCC);
#define ccclose()  pwm_setduty(255) //_set_bit(PORTB,ucCC);
#else
#define ccstart()  _clear_bit(PORTB,ucCC)
#define ccclose()  _set_bit(PORTB,ucCC)
#endif

unsigned char duty=240; 
int main()
{
    unsigned int adc = 0;

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */

  
    led_init();
    uckey_init();
	adc_init();	
#ifdef PWM
    pwm_init();
#endif
    ccclose();
	while (1){

 	    int loop =0,count=0;
	    loop=0;

		if(count==0)
		{
 		   
           while(keydown()){
		     ucLED_Off();  
			 _delay_ms(100);
		     _delay_ms(100);
			 _delay_ms(100);
		     _delay_ms(100);
		     
		     if(keydown()){
			    count+=10;
				ucLED_On();
			 }
		         
		   }
		}
		
		 _delay_ms(100);


		 
		
		 if(count>0)     
           ccstart();


		  for(;loop<10*count;loop++) //one minites,4 loop
		  {
		      sharp_flash(); //1 secs
			    if(keydown()){
		  	       duty +=20;

		       }

			 ccclose();
	        _delay_ms(100);  _delay_ms(100);	
	        _delay_ms(100);  _delay_ms(100);
			_delay_ms(100);	
	        
			adc=_adc(ucADC_CH);
		    if(adc>970){
			   ccstart();
			   _delay_ms(30);
		   	   ccclose();
	           _delay_ms(30);
 		       ccstart();
               _delay_ms(30);
			   ccclose();
	           _delay_ms(30);
			   ccstart();
               _delay_ms(30);
			   ccclose();
	           _delay_ms(30);

			  }
			ccstart();
			if(adc>987){ //stop, test result:4.18V
			   count=0;
			   ccclose();
			}
		  }

          count=0;
	//	  pwm_setduty(duty);
		  ccclose();

		  	 
         
	}
}
