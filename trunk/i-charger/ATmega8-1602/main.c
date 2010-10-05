/*
 *Fuse config:
 * 	CKSEL: 0011   内部时钟， 4M， 其他默认
 *
 *
 *   
 */
#include "include/bitops.h"
#include "include/avrio.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pwm.h"
#include "charger.h"


i_charger charger;

#define KEY_PORT PORTD
#define KEYUP  2
#define KEYDOWN  3
#define _key_init() \
			 _pin_mode(KEY_PORT,KEYUP, INPUT);\
			 _pin_mode(KEY_PORT,KEYDOWN, INPUT)


#define _test_key(n) 								\
		if( ! _test_bit(_inb(KEY_PORT), n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(KEY_PORT), n) )		\
				return 1;							\
        }											\
       return 0										\


char key(char key)
{
   _test_key(key);
}

char i=0;

/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/
void print10(unsigned short n);
void updata_lcd(void)
{
   long x;
   //first line
   ic_update_lcd(&charger);

   //second line
   lcd_cursor(0,1);
   x = adc_A()*1000;
   print10(x);
   lcd_puts("mA");
   lcd_cursor(9,1);
   x= adc_V()*1000;
   print10(x);
   lcd_puts("mV");
}



int main()
{

    char force_stop = 0;
	char pwm =0;
	
	init_ic(&charger);

    cli();
    led_init();
    _key_init();

	pwm_init();
	pwm_setduty(0);

    adc_init();


	lcd1602_init();

	timer0_init();
	enable_timer0();
    sei();

	    
	while (1){
	    if(!force_stop)
		   charging(&charger);

		updata_lcd();
	
		if( key(KEYDOWN)){
            pwm =  pwm_getduty();
			pwm_setduty(0);
			force_stop = 1;
		 }  
		 if( key(KEYUP)){
            
			pwm_setduty(pwm);
			force_stop = 0;
		  
		 }  
	}
}
