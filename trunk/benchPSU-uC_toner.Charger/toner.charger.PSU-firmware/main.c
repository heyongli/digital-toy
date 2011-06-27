#include "config.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include <io.h>
#include  <bitops.h>




/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/

int main()
{
	timer0_init();
	enable_timer0();
	lcd1602_init();
	adc_init();
	iron_init();
	
	lcd_puts("Tool gear V0");
    sti();

	while(1){
		update_lcd();
		iron_loop();
	}

}
