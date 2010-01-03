#include "include/avrio.h"
#include <util/delay.h>

#define LED_PORT PORTD

#define LED_On(i)  _clear_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */
#define LED_Off(i) _set_bit(LED_PORT,i)	/* 输出低电平，使LED发光 */




void led_init()
{
	/* 往前步进 */
    char i;
		for (i = 0; i < 3; i++){

			_pins_mode(LED_PORT,0,1,OUTPUT);//_nm8(0b11,0,1);
	//		_delay_ms(200);
 	//	    LED_Off(0);
 	//	    LED_Off(1);
	//		_delay_ms(200);
		}

}	


char mod=2;

#if 0
void sharp_flash()
{
    char x;

    for(x=0;x<19;x++){
   	   LED_On(mod&2?0:7);
	   LED_On(mod&4?1:7);

       _delay_ms(20);
	 

	   LED_Off(0);
 	   LED_Off(1);
	   	_delay_ms(20);
	}
   mod++;
}
#endif
