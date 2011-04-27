#include "config.h"

#include <avr/interrupt.h>
#include <util/delay.h>


#include  <atmel/avr-io.h>
#include  <bitops.h>




/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/

int main()
{

  	cli();
	lcd1602_init();
	

      
	while(1){
		freq_main();

	}

}
