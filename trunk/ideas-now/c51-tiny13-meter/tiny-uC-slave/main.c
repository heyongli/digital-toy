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

	while (1){

 	    sharp_flash();      
	}
}
