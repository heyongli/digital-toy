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


unsigned char duty=0; 

int main()
{
    unsigned int leye,reye = 0;

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */

    pwm_init();
	adc_init();	



	L_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	L_MOTOR = 150;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	L_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
		L_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	L_MOTOR = 150;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	L_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;

	R_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	R_MOTOR = 150;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	R_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;	
	R_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	R_MOTOR = 150;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;
	R_MOTOR = 255;
	_delay_ms(100);
	L_MOTOR=R_MOTOR = 0;	

    led_init();

	while (1){

 	        
	   leye=_adc(LEYE_ADC);
	   reye=_adc(REYE_ADC);
	   
	    if(leye>1000 || reye>1000){
			ucLED_On();
	    }else
		     ucLED_Off();
		   
		
        L_MOTOR = (LEYE_ADC/4-50);
		R_MOTOR = (REYE_ADC/4-50);
         
	}
}
