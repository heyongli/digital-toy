/*
 *Fuse config:
 * 	CKSEL: 0011   内部时钟， 4M， 其他默认
 *
 *
 *   
 */
#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"
#include "bitops.h"

#define _key_init(x,n) DDR##x &= ~(1<< DD##x##n);  /*enable input*/   \
					   PORT##x |= (1<< P##x##n)    /*pull-up-enable*/  

#define _test_key(x,n) 								\
		if( ! _test_bit(PIN##x, P##x##n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(PIN##x, P##x##n) )		\
				return 1;							\
        }											\
       return 0										\


char keydown()
{
   _test_key(D,3);
}
char keyup()
{
   _test_key(D,2);
}

unsigned char duty=128; 

int main()
{

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */
    DDRD = 0xFF;
    PORTD = 0xFF;	/* 关闭全部LED */
    
    led_init();
    _key_init(D,3);	

    
	pwm_init();

    
	while (1){
	   	       
        //sharp_flash();
	    //pwm_demo();
        if( keydown()){
		  //_set_bit(PORTB,1);
		  duty-=2;

 		  pwm_setduty(duty);
		  if(duty>=0xFF)
		     duty=0;
          _clear_bit(PORTD,1);
		  _delay_ms(20);  
		  _set_bit(PORTD,1);
		 }  
		 if( keyup()){
		  //_set_bit(PORTB,1);
		  duty+=2;

 		  pwm_setduty(duty);
		  if(duty<=0)
		     duty=0xFF;
          _clear_bit(PORTD,0);
		  _delay_ms(20);  
		  _set_bit(PORTD,0);
		 }  
	}
}
