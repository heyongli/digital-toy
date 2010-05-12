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

unsigned char duty=128; 

int main()
{
    unsigned int adc = 0;

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */

  
    led_init();
    uckey_init();
	adc_init();	
    
	//pwm_init();

    pwm_setduty(255);
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
           _clear_bit(PORTB,ucCC);

		  for(;loop<6*count;loop++) //one minites,4 loop
		  {
		    sharp_flash(); //15 secs
			 

			_set_bit(PORTB,ucCC);
	        _delay_ms(100);        _delay_ms(100);	
	        _delay_ms(100);	
	        _delay_ms(100);        _delay_ms(100);	
	        _delay_ms(100);        _delay_ms(100);	
	        _delay_ms(100);	
            
			adc=_adc(ucADC_CH);
		    if(adc>970){
			   _clear_bit(PORTB,ucCC);
			   _delay_ms(30);
		   	   _set_bit(PORTB,ucCC);
	           _delay_ms(30);
 		       _clear_bit(PORTB,ucCC);
               _delay_ms(30);
			   _set_bit(PORTB,ucCC);
	           _delay_ms(30);
			   _clear_bit(PORTB,ucCC);
               _delay_ms(30);
			   _set_bit(PORTB,ucCC);
	           _delay_ms(30);


			}
			_clear_bit(PORTB,ucCC);
			if(adc>987){ //stop, test result:4.18V
			   count=0;
			}
		  }
		  duty-=30;
          count==0;
	//	  pwm_setduty(duty);
		  _set_bit(PORTB,ucCC);

		  	 
         
	}
}
