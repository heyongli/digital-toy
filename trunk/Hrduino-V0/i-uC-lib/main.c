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
     //system timer init
	//timer0_init();
	//enable_timer0();
    //sei();

     //key LCD shiled
	lcd1602_init();
	//lcd_puts(KEY_SHILD_LOGO);
	//lcd_cursor(0,1);
    //lcd_puts(KEY_SHILD_LOGO1);
	//_key_init();

   //  while(1);
 	freq_main();

      
	//_pins_mode(LED_PORT,0,2,OUTPUT);//_nm8(0b11,0,1);
	while(1){
	
	 //	demo_loop();
	
	}

}
