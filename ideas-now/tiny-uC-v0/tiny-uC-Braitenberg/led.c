
#include "bitops.h"
#include <avr/io.h>
#include <util/delay.h>
#include "tiny-uc.h"



void led_init()
{
	/* ÍùÇ°²½½ø */
    char i;
	for (i = 0; i < 5; i++){
        ucLED_On();
		_delay_ms(20);
 	    ucLED_Off();
 	    _delay_ms(20);
	}

}	


char mod=2;


void sharp_flash()
{
    char x;

    for(x=0;x<100;x++){
       ucLED_On();
	   
       _delay_ms(10);
	   ucLED_Off();
	   	_delay_ms(10);
	}
    
}
