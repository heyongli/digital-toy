/*
 *Fuse config:
 * 	CKSEL: 0011   内部时钟， 4M， 其他默认
 *
 *
 *   
 */
#include "include/bitops.h"
#include "include/avrio.h"
#include <util/delay.h>
#include "pwm.h"


#define KEY_PORT PORTD
#define KEY1  2
#define KEY2  3
#define _key_init() \
			 _port_mode(KEY_PORT,KEY1, INPUT);\
			 _port_mode(KEY_PORT,KEY2, INPUT)


#define _test_key(n) 								\
		if( ! _test_bit(_inb(KEY_PORT), n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(KEY_PORT), n) )		\
				return 1;							\
        }											\
       return 0										\


char keydown()
{
   _test_key(KEY2);
}
char keyup()
{
   _test_key(KEY1);
}

unsigned char duty=128; 
char i=0;

int main()
{

    led_init();
    _key_init();	

    
	pwm_init();
    //init_74hc595();
    
	
	while (1){
	 
	 
	    //write_74hc595(i++);
	
	
        //sharp_flash();
	    //pwm_demo();
        if( keydown()){
	
	        lcd1602_init();
			lcd_cursor(1,0);
		    lcd_puts("***Digital TOY");
			lcd_scroll(-1);
	
		  //_set_bit(PORTB,1);
		  duty-=2;

 		  pwm_setduty(duty);
		  if(duty>=0xFF)
		     duty=0;
          _clear_bit(PORTD,1);
		  _delay_ms(15);  
		  _set_bit(PORTD,1);
		  _delay_ms(15);  

		 }  
		 if( keyup()){
	        lcd1602_init();
			lcd_cursor(1,1);
		    lcd_puts("Welcom Atmega8");
			lcd_scroll(1);
		  //_set_bit(PORTB,1);
		  duty+=2;

 		  pwm_setduty(duty);
		  if(duty<=0)
		     duty=0xFF;
          _clear_bit(PORTD,0);
		  _delay_ms(15);  
		 _set_bit(PORTD,0);
		  _delay_ms(15);  
		 }  
	}
}
