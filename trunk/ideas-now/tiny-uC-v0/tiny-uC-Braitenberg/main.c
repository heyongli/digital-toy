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

test_motor(char *pwm)
{
	*pwm = 80;   /*top*/
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	
	*pwm=0;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	
	*pwm=50;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	
	*pwm = 40;
	_delay_ms(100);
	_delay_ms(100);
	
	*pwm = 0;
	_delay_ms(100);
	_delay_ms(100);
	
	*pwm = 25;     //1.2*4  slower nearly stop
	_delay_ms(100);
	_delay_ms(100);
	*pwm=0;
}

void set_speed(unsigned int adc, char* ch)
{
  
		*ch=adc;

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

	led_init();	
    test_motor((char*)&R_MOTOR);
    test_motor((char*)&L_MOTOR);

   
	
	while (1){

 	   //continue;
	  
	   leye=_adc(LEYE_ADC);
	   leye+=_adc(LEYE_ADC);
	   leye/=2;
	   reye=_adc(REYE_ADC);
	   reye+=_adc(REYE_ADC);
	   reye/=2;
	   /*sun, in door, 100-240*/
	    if(leye<50 || reye<50){
			ucLED_On();
	    }else
		     ucLED_Off();
		   
       set_speed(150-leye/2,(char*)&L_MOTOR);
       set_speed((150-reye/2),(char*)&R_MOTOR);

		 
	}
}
