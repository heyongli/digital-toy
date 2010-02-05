#include <avr/io.h>
#include <util/delay.h>


#include "pwm.h"



#define _key_init(x,n) DDR##x &= ~(1<< DD##x##n);    \
					   PORT##x |= (1<< P##x##n)/*pull-up-enable*/  

#define _test_key(x,n) 								\
		if( ! (PIN##x&(1<<P##x##n) )){					\
	    	_delay_ms(3); 							\
	    	if(! (PIN##x&(1<<P##x##n) ))	\
				return 1;							\
        }											\
       return 0									\

char keydown()
{
   _test_key(B,3);
}

#define LED_On(i)  PORTB &=~(1<<(i));	/* 输出低电平，使LED发光 */
#define LED_Off(i)  PORTB |=(1<<(i));	/* 输出低电平，使LED发光 */

char mod=1;


void led_test()
{
	/* 往前步进 */
    char i;
		for (i = 0; i < 3; i++){

			PORTB = 0;
			_delay_ms(200);
 		    LED_Off(0);
			LED_Off(1);
 		    LED_Off(2);

			_delay_ms(200);
		}

	

}	

void sharp_flash()
{
    char x;

    for(x=0;x<7;x++){
       LED_On(mod&1?0:7);
	   LED_On(mod&2?1:7);
	   LED_On(mod&4?2:7);

       _delay_ms(10);
	   LED_Off(0);
	   LED_Off(1);
 	   LED_Off(2);
	   	_delay_ms(10);
	}
    if( keydown()){
          mod++;
		  if(mod>=7) 
	  		mod=1;  
		   
	}
}
int main()
{

	DDRB = 0xFF;	/* 定义B口为输出*/
	PORTB = 0xFF;	/* 关闭全部LED */

  
    led_test();
    _key_init(B,3);	

	while (1){
	   

        sharp_flash();
          
	}
}
