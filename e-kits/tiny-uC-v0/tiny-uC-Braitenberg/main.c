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

test_motor(char pwm)
{
	pwm = 80;
	_delay_ms(100);
	pwm=0;
	_delay_ms(100);
	_delay_ms(100);
	
	pwm=50;
	_delay_ms(100);
	pwm = 0;
	_delay_ms(100);
	_delay_ms(100);
	
	pwm = 25;
	_delay_ms(100);
	pwm = 0;
	_delay_ms(100);
	_delay_ms(100);
	
}

int main()
{
    unsigned int leye,reye = 0;

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0;	/* 关闭全部LED */

    pwm_init();
	adc_init();	
    _pin_mode(PORTB,PB2,INPUT);
	_pin_mode(PORTB,PB3,INPUT);

    test_motor(R_MOTOR);
    test_motor(L_MOTOR);

    led_init();
	
	while (1){

 	        
	   leye=_adc(LEYE_ADC);
	   reye=_adc(REYE_ADC);
	   
	    if(leye>800 || reye>800){
			ucLED_On();
	    }else
		     ucLED_Off();
		   
		if(leye<150) 
		  L_MOTOR=0;
		else{
		   unsigned char pwm = leye>>2;

           L_MOTOR = pwm>120?(pwm/3):pwm/2;
		}

        if(reye<150) R_MOTOR=0;
		else{
		   unsigned char pwm = leye>>2;

           R_MOTOR = pwm>120?(pwm/3):pwm/2;
		}
		 
	}
}
