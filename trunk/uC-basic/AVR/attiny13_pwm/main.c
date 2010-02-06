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
   _test_key(B,3);
}

unsigned char duty=128; 

int main()
{

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */

  
    led_init();
    _key_init(B,3);	

    
	pwm_init();

    
	while (1){
	           
       // sharp_flash();
	    //pwm_demo();
        if( keydown()){
		  //_set_bit(PORTB,1);
		  duty+=3;

		  pwm_setduty(duty);
		  if(duty>=0xFF)
		     duty=0;
          _clear_bit(PORTB,2);
		  _delay_ms(20);  
		  _set_bit(PORTB,2);
		 }  
	}
}
