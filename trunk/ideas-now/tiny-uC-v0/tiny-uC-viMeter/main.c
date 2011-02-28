#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

#include "tiny-uc.h"

#define uckey_init()  _pin_mode(PORTB,ucKEY,INPUT)

#define _test_key(x,n) 								\
	

#if 0
char keydown()
{
	if( ! _test_bit(_inb(PORTB),ucKEY)) {			
	    	_delay_ms(7); 							
	    	if(! _test_bit(_inb(PORTB),ucKEY ))		
				return 1;							
        }											
       return 0	;									
}
#endif


unsigned char duty=240; 
int main()
{
    unsigned int adc = 0;

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */


	adc_init();	


	while (1){

 	    int loop =0,count=0;
	    loop=0;

		adc= _adc(0);
		 _delay_ms(100);


		 segprint10(adc*1000);
		 adc= _adc(0);
		 segprint10(adc*1000);
		 
         ms_scan_segvled();
		  	 
         
	}
}
