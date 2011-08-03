#include "config.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <io.h>
#include <bitops.h>
#include <uprint.h>
#include <drivers/lcd.h>

/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/

main()
{
	DDRB=0xFF;
	lcd1602_init();
	adc_init();

   	while(1){
		long adc, v, i;
		lcd_cursor(0,0);

		//debug only show the raw voltage get from the AMP out
    	adc=_adc(2);

		v = ( (adc*5)*5000/1023);//ref volatage 5V, external voltage/2
   		//print10L(v, 5,2);
   		//lcd_putc('V');
        

		adc=_adc(3);
        i= (adc*5*100000/1023)/2; /*current voltage AMP 20 times, 0.1R*/
		//print10L(i, 5,3);
   		lcd_putc('A');
	}

}

